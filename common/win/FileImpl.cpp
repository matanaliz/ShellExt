#include <Logger.h>
#include "FileImpl.h"
#include <sstream>

FileImpl::FileImpl(const std::wstring& path) 
	: m_filePath(path)
	, m_fileDate(L"")
	, m_fileSize(L"")
	, m_fileName(L"")
{
	//m_fileHandle = CreateFile(m_filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
 //       OPEN_EXISTING, 0, NULL);

	//if(m_fileHandle != INVALID_HANDLE_VALUE)
	//{
	prepareInfo();
	//}
	//else
	//{
	//	throw std::invalid_argument("Cannot open file");
	//}
}

FileImpl::~FileImpl()
{
	//CloseHandle(m_fileHandle);
}

void FileImpl::LogInfo()
{
	//TODO 
	//CHecksum
	//async log
	std::wstringstream wss;
	std::wstring spc(L"    ");
	wss.write(m_filePath.c_str(), m_filePath.size());
	wss.write(spc.c_str(), spc.size());
	wss.write(m_fileDate.c_str(), m_fileDate.size());
	wss.write(spc.c_str(), spc.size());
	wss.write(m_fileSize.c_str(), m_fileSize.size());
	Logger::GetInstance()->LogIt(wss.str());
}

void FileImpl::prepareInfo()
{
	//TODO: check for result
	WIN32_FILE_ATTRIBUTE_DATA lpFileInformation;
	BOOL res = GetFileAttributesEx(m_filePath.c_str(), GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &lpFileInformation);
	if (0 == res)
	{
		MessageBox(NULL, std::to_wstring(GetLastError()).c_str(), L"GetFileAttributesEx Error", 0);
	}

	ULONG fileSize = (static_cast<ULONGLONG>(lpFileInformation.nFileSizeHigh) <<
                      sizeof(lpFileInformation.nFileSizeLow) * 8) |
                     lpFileInformation.nFileSizeLow;
	ULONG fileSizeKB = (fileSize / 1024) == 0 ? 1 : fileSize / 1024;
	m_fileSize = std::to_wstring(fileSizeKB);
	m_fileSize.append(L" KB");
	
	SYSTEMTIME lSystemTime;
	FILETIME ftCreationTime = lpFileInformation.ftCreationTime;
	res = FileTimeToSystemTime(&ftCreationTime, &lSystemTime);
	if (0 == res)
	{
		MessageBox(NULL, std::to_wstring(GetLastError()).c_str(), L"FileTimeToSystemTime Error", 0);
	}


	std::wstring date;
	date.resize(20, 0);
	res = GetDateFormat(LOCALE_USER_DEFAULT, NULL, &lSystemTime, L"dd','MM','yyyy", &date[0], 20);
	if (0 == res)
	{
		MessageBox(NULL, std::to_wstring(GetLastError()).c_str(), L"GetDateFormat Error", 0);
	}

	std::wstring time;
	time.resize(10, 0);
	res = GetTimeFormatEx(LOCALE_NAME_USER_DEFAULT, TIME_FORCE24HOURFORMAT, &lSystemTime, L"HH:mm:ss", &time[0], 10);
	std::wstringstream wss;
	wss.unsetf(std::wstringstream::skipws);
	wss << std::noskipws << date << time;
	m_fileDate.swap(wss.str());
}

