#pragma once
#ifndef LOG_H
#define LOG_H

#include "basiclogger.h"
#include "backtrace.h"

#include <iostream>
#include <string>

namespace logg
{
	extern basic_logger e;
	extern basic_logger d;

	extern std::string callstack();

	// Put this under logg engine init
	static basic_logger e = basic_logger(std::cerr);
	static basic_logger d = basic_logger(std::cout);
	

	static std::string callstack()
	{
		static backtrace trace = backtrace();
		return trace.callstack();
	}
} // namespace logg

#endif