#pragma once
#include <string>

class FileImpl;
class File
{
public:
	File(const std::wstring& path);
	~File();

	void LogInfo();

	const std::wstring& Date();
	const std::wstring& Size();
	const std::wstring& Name();
	const std::wstring& Path();

private:
	FileImpl* m_impl;
};