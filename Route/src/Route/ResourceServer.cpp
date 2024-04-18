#include "pch.h"
#include "../pch.h"
#include "ResourceServer.h"
#include "Application.h"
#include "Logger.h"
#include "Image.h"
#include "Texture.h"
#include "Material.h"
#include "Shader.h"
#include "StorageBuffer.h"

#include <list>
#include "Performance.h"

/*
* BOILER PLATE
*/

namespace route
{
	static constexpr RID ChunkBShift = 20;
	static constexpr RID ChunkMask = ~((1u << ChunkBShift) - 1); // maximum possible number of chunks is 4096
	static constexpr RID ElementMask = ~ChunkMask;

	static int *g_SeedStartPtr = new int;
	static RID g_GlobalSeedHashHole =
		static_cast<RID>(_rotl64( 0x981faca1ULL ^ reinterpret_cast<uintptr_t>(g_SeedStartPtr), reinterpret_cast<uintptr_t>(g_SeedStartPtr) & 0x3fU ));

	struct Application::TRSH
	{
	public:
		template <typename... _Tys>
		static inline void execute( bool state ) {
			return state ? open<_Tys...>() : close<_Tys...>();
		}

		static inline void execute( bool state );

		template <typename _Ty>
		static inline void open() {
			ResourceServer<_Ty>::_open();
		}

		template <typename _Ty>
		static inline void close() {
			ResourceServer<_Ty>::_close();
		}

		// two single parameters for overloading (packed args are optional which is not overloaded)
		template <typename _Ty, typename _Ey, typename... _Tys>
		static inline void open() {
			ResourceServer<_Ty>::_open();
			ResourceServer<_Ey>::_open();
			if constexpr (sizeof...(_Tys))
				TRSH::open<_Tys...>();
		}

		// two single parameters for overloading (packed args are optional which is not overloaded)
		template <typename _Ty, typename _Ey, typename... _Tys>
		static inline void close() {
			ResourceServer<_Ty>::_close();
			ResourceServer<_Ey>::_close();
			if constexpr (sizeof...(_Tys))
				TRSH::close<_Tys...>();
		}
	};

	inline void Application::TRSH::execute( bool state ) {
		const auto mem = Performance::get_memory_usage();

		/*
		* Template defined below are which resource servers are to be template instanced
		* Not the best place to put such thing, but it works well
		*/
		execute<Resource, Texture, Material, Shader, StorageBuffer>( state );

		std::cout << "mem usage diff after " << (state ? "running" : "closing") << " the ResServers: "
			<< (Performance::get_memory_usage() - mem) << '\n';

		return;
	}

	void Application::_toggle_resource_servers( uint32_t flags ) {
		// TODO: add named flags
		const bool tobe_opened = flags & 1;
		return Application::TRSH::execute( tobe_opened );
	}

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
}

/*
* ACTUAL STUFF
*/

namespace route
{

	template<typename _Ty>
	struct ResourceServer<_Ty>::Internal {
		static_assert(std::is_same_v<Resource, _Ty> || std::is_base_of_v<Resource, _Ty>, "_TY should derive from Resource");

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

		inline Internal() {
			// magic nomber, just for now
			chunks.reserve( 32U );
		}

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
			return { static_cast<uint16_t>((rid ^ seed) >> ChunkBShift), static_cast<uint16_t>((rid ^ seed) & ElementMask) };
		}

		inline RID to_rid( const RIndex rindex ) const {
			return ((rindex.chunk << ChunkBShift) | (rindex.element & ElementMask)) ^ seed;
		}

		inline _Ty &get_resource( const RID rid ) {
			const auto rindex = get_rindex( rid );
			return *reinterpret_cast<_Ty *>(chunks[ rindex.chunk ].elements[ rindex.element ].mem);
		}

		template <typename _Ey>
		inline errno_t _add_resource_unchecked( _Ey resource ) {
			using raw_ey_type = std::_Remove_cvref_t<_Ey>;

			const index_t chunk_index = unlocked_chunk_or_create();

			Chunk &chunk = chunks[ chunk_index ];
			const index_t add_index = chunk.counter();

			// after the last filled element is a filled element????
			// something fucked up, kill me
			if (chunk.elements[ add_index ].flags & EFlag_Init)
			{
				return RIDnpos;
			}

			chunk.inc_counter(); /* can we afford to check for de-syncs?? */

			new (chunk.elements[ add_index ].mem) raw_ey_type( resource );
			chunk.elements[ add_index ].flags = EFlag_Init;

			return to_rid( { static_cast<uint16_t>(chunk_index), static_cast<uint16_t>(add_index) } );
		}

		template <typename _Ey>
		inline errno_t add_resource( _Ey resource ) {
			using raw_ey_type = std::_Remove_cvref_t<_Ey>;
			static_assert(sizeof( raw_ey_type ) <= Element::value_size, "Type too big");
			static_assert(is_related_v<resource_type, raw_ey_type>, "Unrelated type");

			// checking for exceptions
			if constexpr (std::is_lvalue_reference_v<_Ey> && !std::is_copy_constructible_v<raw_ey_type>)
			{
				char msg[ 512 ]{};
				sprintf_s( msg, "illegal construct of type %s: can't be copy-constructible", typeid(raw_ey_type).name() );
				std::_Xruntime_error( msg );
			}
			else if constexpr (std::is_rvalue_reference_v<_Ey> && !std::is_move_constructible_v<raw_ey_type>)
			{
				char msg[ 512 ]{};
				sprintf_s( msg, "illegal construct of type %s: can't be move-constructible", typeid(raw_ey_type).name() );
				std::_Xruntime_error( msg );
			}
			else
			{
				// if the resource is an rvalue, then move it to keep it an rvalue
				if constexpr (std::is_rvalue_reference_v<_Ey>)
					return _add_resource_unchecked<_Ey>( std::move( resource ) );
				else
					return _add_resource_unchecked<_Ey>( resource );
			}
		}

		const RID seed = (g_GlobalSeedHashHole += g_GlobalSeedHashHole * 0xc14a + 1);
		size_t ref_counter;
		vector<Chunk> chunks{};
	};

	// no need to check if the internals are null, let the user figure it out
	template<typename _Ty>
	typename ResourceServer<_Ty>::resource_type &ResourceServer<_Ty>::get_resource( RID rid ) {
		return s_internal->get_resource( rid );
	}

	template<typename _Ty>
	RID ResourceServer<_Ty>::add_resource( resource_type &&resource ) {
		if (!s_internal)
		{
			// TODO: type name in the error msg, please?
			Logger::write( "from add_resource: ResourceServer didn't initalize" );
			return RIDnpos;
		}

		return s_internal->add_resource<resource_type &&>( std::move( resource ) );
	}

	template<typename _Ty>
	RID ResourceServer<_Ty>::add_resource( const resource_type &resource ) {
		if (!s_internal)
		{
			// TODO: type name in the error msg, please?
			Logger::write( "from add_resource: ResourceServer didn't initalize" );
			return RIDnpos;
		}

		return s_internal->add_resource<const resource_type &>( resource );
	}

	template<typename _Ty>
	void ResourceServer<_Ty>::pop_resource( RID rid ) {
		if (!s_internal)
		{
			// TODO: type name in the error msg, please?
			Logger::write( "from pop_resource: ResourceServer didn't initalize" );
			return;
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
			return;
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
	errno_t ResourceServer<_Ty>::_open() {
		if (s_internal)
		{
			s_internal->ref_counter++;
			return 0;
		}

		s_internal = new Internal();
		s_internal->ref_counter = 1;

#ifdef VERBOSE
		std::cout << "VERBOSE: " << typeid(ResourceServer<_Ty>).name() << " opened\n";
#endif

		return 0;
	}

	template<typename _Ty>
	bool ResourceServer<_Ty>::_close() {
		// no internals
		if (!s_internal)
		{
			char buf[ 256 ]{};
			(void)sprintf_s( buf, "%s already closed!", typeid(ResourceServer<_Ty>).name() );
			std::_Xruntime_error( buf );
			//return false;
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
			//return false;
		}

		// no more references to this resource server
		if (!s_internal->ref_counter)
		{
#ifdef VERBOSE
			size_t internals_size = sizeof( Internal );
			internals_size += sizeof( Internal::Chunk ) * s_internal->chunks.size();
#endif

			delete s_internal;
			s_internal = nullptr;

#ifdef VERBOSE
			std::cout << "VERBOSE: " << typeid(ResourceServer<_Ty>).name() << " closing (" << (internals_size >> 10) << " kb)\n";
#endif

			// the internals are deleted
			return true;
		}

		// the internals are still there
		return false;
	}
}
