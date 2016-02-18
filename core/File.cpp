#include <File.h>
#include <FileImpl.h>
#include <memory>
#include <iostream>

File::File(const std::wstring& path) : m_impl(0)
{
	try
	{
		m_impl = new FileImpl(path);
	}
	catch (const std::exception& e)
	{
		std::cerr << "File::File() " << e.what() << std::endl;
	}
}

unsigned long File::LogInfo()
{
	return m_impl->LogInfo();
}

File::~File()
{
	delete m_impl;
}

const std::wstring& File::Date() const
{
	return m_impl->Date();
}

const std::wstring& File::Size() const
{
	return m_impl->Size();
}

const std::wstring& File::Name() const
{
	return m_impl->Name();
}

const std::wstring& File::Path() const
{
	return m_impl->Path();
}