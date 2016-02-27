#pragma once
#ifndef BACKTRACE_H
#define BACKTRACE_H

#include <string>
#include <memory>

class backtrace_impl;
class backtrace
{
public:
	backtrace();
	~backtrace();

	std::string callstack();

private:
	std::unique_ptr<backtrace_impl> m_impl;
};

#endif // BACKTRACE_H