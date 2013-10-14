#pragma once
#include <string>
#include <Windows.h>

class FileImpl
{
public:
	FileImpl(const std::wstring& path);
	~FileImpl();

	void LogInfo();

	const std::wstring& Date() const { return m_fileDate; }
	const std::wstring& Size() const { return m_fileSize; }
	const std::wstring& Name() const { return m_fileName; }
	const std::wstring& Path() const { return m_filePath; }

private:

	void prepareInfo();
	DWORD computeSum();

	std::wstring m_filePath;
	std::wstring m_fileDate;
	std::wstring m_fileSize;
	std::wstring m_fileName;

	HANDLE m_fileHandle;

};