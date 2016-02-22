#pragma once
#include <string>
#include <memory>

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
	std::unique_ptr<FileImpl> m_impl;
};