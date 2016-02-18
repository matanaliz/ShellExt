#pragma once
#include <string>

class FileImpl;
class File
{
public:
	File(const std::wstring& path);
	~File();

	unsigned long LogInfo();

	const std::wstring& Date() const;
	const std::wstring& Size() const;
	const std::wstring& Name() const;
	const std::wstring& Path() const;

private:
	FileImpl* m_impl;
};