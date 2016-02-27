#include "backtrace.h"
#include "backtrace_impl.h"
#include <new>

backtrace::backtrace()
: m_impl(new (std::nothrow) backtrace_impl())
{
}

// Dtor cannot be defaulted with forward declarated class
backtrace::~backtrace()
{
}

std::string backtrace::callstack()
{
	// Handle this in other way
	return m_impl ? m_impl->callstack() : "Unable to initialize backtrace implementation";
}
