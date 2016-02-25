#include <File.h>
#include <FileImpl.h>
#include <iostream>
#include <memory>

File::File(const std::wstring& path) 
	: m_impl(std::make_unique<FileImpl>(path))
{
}

unsigned long long File::GetCheckSum()
{
	return m_impl ? m_impl->computeSum() : 0;
}

File::~File()
{
}

const std::wstring& File::Date() const
{
	return m_impl ? m_impl->Date() : std::move(std::wstring());
}

const unsigned long long File::Size() const
{
	return m_impl ? m_impl->Size() : 0;
}

const unsigned long long File::SizeKb() const
{
	return m_impl ? m_impl->SizeKb() : 0;
}

const std::wstring& File::Path() const
{
	return m_impl ? m_impl->Path() : std::move(std::wstring());
}