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

File::~File()
{
	delete m_impl;
}

const std::wstring& File::Date()
{
	return m_impl->Date();
}

const std::wstring& File::Size()
{
	return m_impl->Size();
}

const std::wstring& File::Name()
{
	return m_impl->Name();
}