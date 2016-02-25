#pragma once
#include <string>
#include <memory>

class FileImpl;
class File
{
public:
	File(const std::wstring& path);
	~File();

	unsigned long long GetCheckSum();

	const std::wstring& Date() const;
	const unsigned long long Size() const;
	const unsigned long long SizeKb() const;
	const std::wstring& Path() const;

private:
	mutable std::unique_ptr<FileImpl> m_impl;
};