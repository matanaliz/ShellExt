#pragma once
#include <string>
#include <Windows.h>

class FileImpl
{
public:
	FileImpl(const std::wstring& path);
	~FileImpl();

	const std::wstring& Date() const;
	const std::wstring& Size() const;
	const std::wstring& Name() const;

private:

	void prepareInfo();

	std::wstring m_filePath;
	std::wstring m_fileDate;
	std::wstring m_fileSize;
	std::wstring m_fileName;

	HANDLE m_fileHandle;

};