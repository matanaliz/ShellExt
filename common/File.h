#pragma once
#include <string>

class FileImpl;
class File
{
public:
	File(const std::wstring& path);
	~File();

	const std::wstring& Date();
	const std::wstring& Size();
	const std::wstring& Name();

private:
	FileImpl* m_impl;
};