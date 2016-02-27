#pragma once
#ifndef BACKTRACE_IMPL_H
#define BACKTRACE_IMPL_H
#include <string>

class backtrace_impl
{
public:
	backtrace_impl() = default;
	~backtrace_impl() = default;
	backtrace_impl& operator=(const backtrace_impl&) = delete;
	backtrace_impl(const backtrace_impl&) = delete;

	std::string callstack();
};

#endif //BACKTRACE_IMPL_H