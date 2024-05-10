#include "pch.h"
#include "Logger.h"
#include <iostream>
#include <chrono>

using second = std::chrono::seconds;

LogLevel Logger::s_allowed = LogLevel::Debug;

static inline tm log_time() {
	tm long_time;
	time_t short_time;

	(void)time( &short_time );
	(void)localtime_s( &long_time, &short_time );

	return long_time;
}

static inline void log( const char *prefix, const char *text ) {
	tm current = log_time();
	char time_str[ 32 ]{};

	// stringfy time
	(void)asctime_s( time_str + 1, std::size( time_str ) - 1, &current );
	time_str[ 0 ] = '[';

	// remove stupid new line
	for (char *t_str = time_str; *t_str; t_str++)
	{
		if (*t_str == '\n' || *t_str == '\r')
		{
			*t_str = 0;
			break;
		}
	}

	// TODO: log to file

	if (prefix)
	{
		std::cerr << time_str << "] [" << prefix << "] " << text << '\n';
	}
	else
	{
		std::cerr << time_str << "] " << text << '\n';
	}
}

namespace route
{
	void Logger::write( const string &line, LogLevel level ) {
		return write( line.c_str(), level );
	}

	void Logger::write( const char *line, LogLevel level ) {
		if (level < s_allowed)
			return;
		using prefix = char[ 4 ];
		constexpr prefix prefixes[] = {
			"vrb",
			"dbg",
			"inf",
			"wrn",
			"err",

			"OR0",
			"OR1",
			"OR2",
		};
		return log( prefixes[ static_cast<unsigned>(level) ], line );
	}

	void Logger::apply_minimum_log_level( LogLevel level ) {
		s_allowed = level;
	}

	LogLevel Logger::minimum_log_level() {
		return s_allowed;
	}

}
