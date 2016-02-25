#pragma once
#include <string>
#include <Windows.h>

class FileImpl
{
public:
	FileImpl(const std::wstring& path);
	~FileImpl();

	FileImpl(FileImpl&) = delete;
	FileImpl& operator=(const FileImpl&) = delete;

	unsigned long long computeSum();

	const std::wstring&			Date();
	const unsigned long long	Size();
	const unsigned long long	SizeKb();
	const std::wstring&			Path();

private:
	std::wstring m_path;
	std::wstring m_date;
	unsigned long long m_size;
	unsigned long long m_sizeKb;
};