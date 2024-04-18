#pragma once
#include "Bases.h"

namespace route
{

	class Performance
	{
	public:
		struct MemoryUsage
		{
			intptr_t available;
			intptr_t usage; // private
			intptr_t peak_usage; // private
			intptr_t total_commit; // private + shared

			inline MemoryUsage operator+( const MemoryUsage &other ) {
				MemoryUsage result;
				result.available = available + other.available;
				result.usage = usage + other.usage;
				result.peak_usage = peak_usage + other.peak_usage;
				result.total_commit = total_commit + other.total_commit;
				return result;
			}

			inline MemoryUsage operator-( const MemoryUsage &other ) {
				MemoryUsage result;
				result.available = available - other.available;
				result.usage = usage - other.usage;
				result.peak_usage = peak_usage - other.peak_usage;
				result.total_commit = total_commit - other.total_commit;
				return result;
			}
		};

		static MemoryUsage get_memory_usage();

	private:
		Performance() = delete;
		~Performance() = delete;
	};

}

namespace std
{
	inline ostream &operator<<( ostream &out, const route::Performance::MemoryUsage &usage ) {
		return out << '[' << (usage.usage >> 10U) << "KB ("
			<< (usage.total_commit >> 10U) << "KB) / "
			<< (usage.available >> 10U) << "KB]";
	}
}
