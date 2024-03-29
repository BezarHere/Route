#pragma once
#include "Vector.h"
#include "Color.h"

namespace route
{
	template <typename _Ty, typename _Uv = Vec2us>
	struct TVertex
	{
		_Ty pos;
		_Uv uv;
	};

	template <typename _Ty, typename _Clr = Clr, typename _Uv = Vec2us>
	struct TClrVertex
	{
		_Ty pos;
		_Clr clr;
		_Uv uv;
	};

	template <typename _Ty, typename _Uv = Vec2us, typename _Nm = Vec3f>
	struct TNrmVertex
	{
		_Ty pos;
		_Uv uv;
		_Nm nrm;
	};

	using Vrtx2 = TVertex<Vec2f>;
	using Vrtx3 = TVertex<Vec3f>;

	using VrtxC2 = TClrVertex<Vec2f>;
	using VrtxC3 = TClrVertex<Vec3f>;

	using Vrtx8C2 = TClrVertex<Vec2f, Clr8>;
	using Vrtx8C3 = TClrVertex<Vec3f, Clr8>;

	using VrtxN3 = TNrmVertex<Vec3f, Clr8, Vec3f>;

	using Vrtx16N3 = TNrmVertex<Vec3f, Clr8, Vec3s>;

	constexpr int NormalizedVInputTypeBit = 0x10000;

	enum class VertexInputType
	{
		Byte = 0x1400,
		UnsignedByte,
		Short,
		UnsignedShort,
		Int,
		UnsignedInt,

		NormalizedByte = Byte | NormalizedVInputTypeBit,
		NormalizedUnsignedByte = UnsignedByte | NormalizedVInputTypeBit,
		NormalizedShort = Short | NormalizedVInputTypeBit,
		NormalizedUnsignedShort = UnsignedShort | NormalizedVInputTypeBit,
		NormalizedInt = Int | NormalizedVInputTypeBit,
		NormalizedUnsignedInt = UnsignedInt | NormalizedVInputTypeBit,

		HalfFloat = 0x140B,
		Float = 0x1406,
		Double = 0x140A
	};

	enum class VertexInputSize
	{
		Single = 1,

		Double = 2,
		Vec2 = 2,

		Trible = 3,
		RGB = 3,
		Vec3 = 3,

		Qoudruble = 4,
		Vec4 = 4,
		RGBA = 4,
		Color = 4
	};

	struct VInputAttribute
	{
		VertexInputType type;
		VertexInputSize size;
	};

	typedef unsigned VInputDescID;

	struct VertexInputDesc
	{
	public:
		using container_type = vector<VInputAttribute>;

		// 32 seems resonable
		static constexpr size_t MaxVertexAttributes = 32ULL;

		VertexInputDesc();
		~VertexInputDesc();

		VertexInputDesc( const VertexInputDesc &copy );
		VertexInputDesc( VertexInputDesc &&move ) noexcept;
		VertexInputDesc &operator=( const VertexInputDesc &copy );
		VertexInputDesc &operator=( VertexInputDesc &&move ) noexcept;

		// only binds, doesn't apply/enable the vertex array attribs
		void bind() const;

		void setup() const;

		static VInputDescID get_bound();
		static void clear_bound();
		static void clear_setup();

		/// @param stride a stride of zero will be defaulted to the vertex size.
		///	              it's always better to set the stride either zero or greater/equal to the vertex size 
		void set_stride( uint32_t stride );
		void set_offset( uint32_t offset );

		inline VInputDescID get_name() const {
			return m_id;
		}

		inline container_type &get_attrs() {
			return m_container;
		}

		inline const container_type &get_attrs() const {
			return m_container;
		}

		inline uint32_t get_stride() const {
			return m_stride;
		}

		inline uint32_t get_offset() const {
			return m_offset;
		}

		size_t get_vertex_size() const;

	private:
		void _validate_attributes();

	private:
		VInputDescID m_id;
		container_type m_container;
		uint32_t m_offset;
		uint32_t m_stride;
	};

}
