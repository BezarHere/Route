#pragma once
#include "Bases.h"

namespace route
{

	template <typename _Ty>
	struct named
	{
		static constexpr size_t name_capacity = 64;

		using value_type = _Ty;
		using string_character = char;
		using string_storage = array<string_character, name_capacity>;
		using string_type = std::basic_string<string_character>;
		using pair_type = std::pair<string_storage, value_type>;

		inline named() = default;

		inline named( const value_type &copy )
			: name{}, value{ copy } {
		}

		inline named( value_type &&copy ) noexcept
			: name{}, value{ copy } {
		}

		inline named( const pair_type &pair )
			: name{ pair.first }, value{ pair.second } {
		}

		inline operator pair_type() const {
			return { name, value };
		}

		inline operator string_type() const {
			return { name.data(), _name_len() };
		}

		string_storage name;
		value_type value;
	private:
		inline size_t _name_len() const {
			for (uint32_t i = 0; i < name.size(); i++)
			{
				// if we hit the a null-termination, we should return it's index
				if (!name[ i ])
				{
					return i;
				}
			}
			// name is full
			return name.size();
		}
	};

	using named_bool = named<bool>;
	using named_short = named<int16_t>;
	using named_ushort = named<uint16_t>;
	using named_int = named<int32_t>;
	using named_uint = named<uint32_t>;
	using named_long = named<int64_t>;
	using named_ulong = named<uint64_t>;
	using named_float = named<float>;
	using named_double = named<double>;
	using named_ldouble = named<long double>;
}
