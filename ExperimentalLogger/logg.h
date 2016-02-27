#pragma once
#ifndef LOG_H
#define LOG_H

#include <memory>
#include <iostream>
#include <sstream>

// This goes to impl header 
#include <Windows.h>
#include "DbgHelp.h"

namespace logg
{
	class basic_logger;
	extern basic_logger e;
	extern basic_logger d;

	extern std::string trace();

	// Move this to cpp
	class callstack_impl
	{
	public:
		std::string trace()
		{
			unsigned int   i;
			void         * stack[100];
			unsigned short frames;
			SYMBOL_INFO* symbol;
			HANDLE         process;

			process = GetCurrentProcess();
			SymInitialize(process, NULL, TRUE);
			frames = CaptureStackBackTrace(0, 100, stack, NULL);
			symbol = (SYMBOL_INFO *)calloc(sizeof(SYMBOL_INFO)+256 * sizeof(char), 1);
			symbol->MaxNameLen = 255;
			symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

			std::stringstream ss;
			for (i = 0; i < frames; i++)
			{
				SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
				ss << "Frame " << frames - i - 1 << " " << symbol->Name << " " << symbol->Address << std::endl;
			}
			free(symbol);

			return ss.str();
		}
	private:
	};

	class callstack
	{
	public:
		callstack()
			: m_impl(new callstack_impl())
		{}
		~callstack()
		{
		}
		std::string trace()
		{
			return m_impl->trace();
		}

	private:
		std::unique_ptr<callstack_impl> m_impl;
	};


	class basic_logger
	{
	private:
		std::ostream& m_stream;

	public:
		basic_logger(std::ostream& stream = std::cout)
			: m_stream(stream)
		{}

		~basic_logger() {};

		template <typename T>
		basic_logger& operator<<(const T& value)
		{
			m_stream << value;
			return *this;
		}

		// To handle std::endl and other std stream manipulators
		basic_logger& operator<<(std::ostream& (*fp)(std::ostream&))
		{
			fp(m_stream);
			return *this;
		}

		template <typename T>
		friend basic_logger& operator<< (basic_logger& logger, const T& value);
	};

	template <typename T>
	basic_logger& operator<< (basic_logger& logger, const T& value)
	{
		logger.m_stream << value;
		return logger;
	}

	static basic_logger e = basic_logger(std::cerr);
	static basic_logger d = basic_logger(std::cout);
	

	static std::string trace()
	{
		static callstack cstack = callstack();
		return cstack.trace();
	}
} // namespace logg

#endif