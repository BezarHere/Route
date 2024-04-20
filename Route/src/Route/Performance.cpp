#include "pch.h"
#include "Performance.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>

namespace route
{

	Performance::MemoryUsage Performance::get_memory_usage() {
#ifdef WIN32
		APP_MEMORY_INFORMATION info{};
		BOOL success = GetProcessInformation( GetCurrentProcess(), ProcessAppMemoryInfo, &info, sizeof( info ) );

		MemoryUsage result;
		if (!success)
		{
			result.available = -1;
			result.peak_usage = -1;
			result.total_commit = -1;
			result.usage = -1;
			return result;
		}

		result.available = info.AvailableCommit;
		result.peak_usage = info.PeakPrivateCommitUsage;
		result.total_commit = info.TotalCommitUsage;
		result.usage = info.PrivateCommitUsage;
		return result;
#else
		// TODO: linux + others
		return MemoryUsage();
#endif
	}

}
