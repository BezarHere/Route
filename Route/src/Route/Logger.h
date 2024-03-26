#pragma once
#include "Bases.h"

namespace route
{
	enum class LogLevel
	{
		Verbose,
		Debug,
		Info,
		Warnning,
		Error
	};

	class Logger
	{
	public:
		static void write( const string &line, LogLevel level = LogLevel::Info );
		static void write( const char *line, LogLevel level = LogLevel::Info );
		static void apply_minimum_log_level( LogLevel level );
		static LogLevel minimum_log_level();
	private:
		static LogLevel s_allowed;
	};
}
