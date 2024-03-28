#include "pch.h"
#include "../pch.h"
#include "ResourceServer.h"
#include <list>
#include "Image.h"


namespace route
{

	static constexpr RID ChunkBShift = 20;
	static constexpr RID ChunkMask = ~((1u << ChunkBShift) - 1); // maximum possible number of chunks is 4096
	static constexpr RID ElementMask = ~ChunkMask;

	struct RIndex
	{
		uint16_t chunk;
		uint16_t element;
	};

	template<typename _Ty>
	typename ResourceServer<_Ty>::Internal *ResourceServer<_Ty>::s_internal = nullptr;

	enum ElementFlags
	{
		EFlag_None = 0,
		EFlag_Init = 1,
	};

	template<typename _Ty>
	struct ResourceServer<_Ty>::Internal {



		struct Element
		{
			static constexpr size_t value_size = sizeof( _Ty );
			using memory_element = uint8_t;
			static constexpr size_t memory_count =
				value_size / sizeof( memory_element ) + (value_size % sizeof( memory_element ) ? 1 : 0);
			using memory_block = memory_element[ memory_count ];

			inline _Ty &value() {
				return *reinterpret_cast<_Ty *>(mem);
			}

			ElementFlags flags{};
			array<resource_name_char, ResourceNameMaxLn> name{};
			memory_block mem; // <- don't init on default!
		};

		static constexpr size_t ChunkBufferLength = 256;

		struct Chunk
		{
		public:

			inline bool is_locked() const {
				return m_locked;
			}

			inline uintptr_t counter() const {
				return m_elements_counter;
			}

			inline void dec_counter() {
				m_elements_counter--;
				if (!m_elements_counter)
				{
					m_locked = false;
				}
			}

			inline void inc_counter() {
				m_elements_counter++;
				if (m_elements_counter >= elements.size())
				{
					m_locked = true;
				}
			}

			// a chunk becomes locked when it's filled
			// also, a chuck unlocks when it's empty
			array<Element, ChunkBufferLength> elements{};
		private:
			bool m_locked = false;
			uintptr_t m_elements_counter = 0;
		};

		inline index_t first_unlocked_chunk() const {
			for (index_t i = 0; i < chunks.size(); i++)
			{
				if (!chunks[ i ].is_locked())
				{
					return i;
				}
			}
			return npos;
		}

		inline index_t create_chunk() {
			chunks.emplace_back();
			return chunks.size() - 1;
		}

		// gets the first unlocked chunk, if non are found; create one
		inline index_t unlocked_chunk_or_create() {
			const index_t first = first_unlocked_chunk();
			if (first == npos)
			{
				return create_chunk();
			}
			return first;
		}

		inline RIndex get_rindex( const RID rid ) const {
			return { static_cast<uint16_t>(rid >> ChunkBShift), static_cast<uint16_t>(rid & ElementMask) };
		}

		inline RID to_rid( const RIndex rindex ) const {
			return ((rindex.chunk << ChunkBShift) | (rindex.element & ElementMask));
		}

		inline _Ty &get_resource( const RID rid ) {
			const auto rindex = get_rindex( rid );
			return *reinterpret_cast<_Ty *>(chunks[ rindex.chunk ].elements[ rindex.element ].mem);
		}

		size_t ref_counter;
		vector<Chunk> chunks{};
	};

	// no need to check if the internals are null, let the user figure it out
	template<typename _Ty>
	typename ResourceServer<_Ty>::resource_type &ResourceServer<_Ty>::get_resource( RID rid ) {
		return s_internal->get_resource( rid );
	}

	template<typename _Ty>
	RID ResourceServer<_Ty>::add_resource( const resource_type &resource ) {
		if (!s_internal)
		{
			// TODO: type name in the error msg, please?
			Logger::write( "from add_resource: ResourceServer didn't initalize" );
			return RIDnpos;
		}

		const index_t chunk_index = s_internal->unlocked_chunk_or_create();

		typename Internal::Chunk &chunk = s_internal->chunks[ chunk_index ];
		const index_t add_index = chunk.counter();

		// after the last filled element is a filled element????
		// something fucked up, kill me
		if (chunk.elements[ add_index ].flags & EFlag_Init)
		{
			return RIDnpos;
		}

		chunk.inc_counter(); /* can we afford to check for de-syncs?? */
		new (chunk.elements[ add_index ].mem) resource_type( resource );
		chunk.elements[ add_index ].flags = EFlag_Init;

		return s_internal->to_rid( { static_cast<uint16_t>(chunk_index), static_cast<uint16_t>(add_index) } );
	}

	template<typename _Ty>
	void ResourceServer<_Ty>::pop_resource( RID rid ) {
		if (!s_internal)
		{
			// TODO: type name in the error msg, please?
			Logger::write( "from pop_resource: ResourceServer didn't initalize" );
			return RIDnpos;
		}

		const RIndex rindex = s_internal->get_rindex( rid );

		if (rindex.chunk >= s_internal->chunks.size())
			return;

		if (rindex.element >= Internal::ChunkBufferLength)
			return;

		typename Internal::Element &element = s_internal->chunks[ rindex.chunk ].elements[ rindex.element ];
		element.value().~resource_type();
		element.flags = EFlag_None;

		s_internal->chunks[ rindex.chunk ].dec_counter();
	}

	template<typename _Ty>
	void ResourceServer<_Ty>::set_resource_name( RID rid, const resource_name_char *name ) {
		if (!s_internal)
		{
			// TODO: type name in the error msg, please?
			Logger::write( "from set_resource_name: ResourceServer didn't initalize" );
			return RIDnpos;
		}

		const RIndex rindex = s_internal->get_rindex( rid );
		(void)strncpy_s(
			s_internal->chunks[ rindex.chunk ].elements[ rindex.element ].name.data(),
			ResourceNameMaxLn,
			name,
			_TRUNCATE
		);
	}

	template<typename _Ty>
	errno_t ResourceServer<_Ty>::open() {
		if (s_internal)
		{
			s_internal->ref_counter++;
			return 0;
		}

		s_internal = new Internal();
		s_internal->ref_counter = 1;
		return 0;
	}

	template<typename _Ty>
	bool ResourceServer<_Ty>::close() {
		// no internals
		if (!s_internal)
		{
			char buf[ 256 ]{};
			(void)sprintf_s( buf, "%s already closed!", typeid(ResourceServer<_Ty>).name() );
			std::_Xruntime_error( buf );
			return false;
		}

		// was the ref_counter zero to overflow? then why isn't it deleted??
		if (--s_internal->ref_counter == npos)
		{
			char buf[ 256 ]{};
			(void)sprintf_s(
				buf,
				"%s has a corrupted internals ref_counter: %llu",
				typeid(ResourceServer<_Ty>).name(),
				s_internal->ref_counter
			);
			std::_Xruntime_error( buf );
			return false;
		}

		// no more references to this resource server
		if (!s_internal->ref_counter)
		{
			delete s_internal;
			s_internal = nullptr;

			// the internals are deleted
			return true;
		}

		// the internals are still there
		return false;
	}
}
