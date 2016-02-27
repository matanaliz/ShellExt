#include "backtrace.h"
#include "backtrace_impl.h"
#include <sstream>

// This goes to impl header 
#include <Windows.h>
#include <DbgHelp.h>

std::string backtrace_impl::callstack()
{
	// Make this configurable
	const std::size_t kMaxFrames = 100;
	std::stringstream ss;

	SYMBOL_INFO* symbol = (SYMBOL_INFO *)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
	std::shared_ptr<SYMBOL_INFO> symbol_ptr = std::shared_ptr<SYMBOL_INFO>(symbol, &std::free);
	if (symbol_ptr)
	{
		symbol_ptr->MaxNameLen = MAX_PATH;
		symbol_ptr->SizeOfStruct = sizeof(SYMBOL_INFO);

		HANDLE process = { GetCurrentProcess() };
		SymInitialize(process, NULL, TRUE);

		void* stack[kMaxFrames];
		std::size_t frames = CaptureStackBackTrace(0, kMaxFrames, stack, NULL);
		for (std::size_t i = 0; i < frames; i++)
		{
			SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol_ptr.get());

			// Make output format configurable
			ss << "Frame " << frames - i - 1
				<< " " << symbol_ptr->Name
				<< " " << symbol_ptr->Address
				<< std::endl;
		}
	}
	else
	{
		ss << "Unable to allocate memory for symbol info" << std::endl;
	}

	return ss.str();
}
