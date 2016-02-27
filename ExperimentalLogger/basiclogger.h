#pragma once
#ifndef BASICLOGGER_H
#define BASICLOGGER_H
#include <iostream>

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

#endif // BASICLOGGER_H