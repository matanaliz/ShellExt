#include <sstream>
#include <fstream>
#include <Logger.h>
#include <ThreadPool.h>
#include "FileImpl.h"


FileImpl::FileImpl(const std::wstring& path) 
	: m_filePath(path)
	, m_fileDate(L"")
	, m_fileSize(L"")
	, m_fileName(L"")
{
	prepareInfo();
}

FileImpl::~FileImpl()
{
}

unsigned long FileImpl::computeSum()
{
	//Probably, I need to use WINAPI for asyc file reading
	unsigned long sum = 0;
	std::fstream fstr;
	fstr.open(m_filePath.c_str(), std::fstream::in | std::fstream::binary);
	if (fstr.is_open())
	{
		while (!fstr.eof() && fstr.good())
		{
			char c;
			fstr.get(c);
			sum += static_cast<unsigned long>(c);
		}
		fstr.close();
	}

	return sum;
}

unsigned long FileImpl::computeSumBuf(const unsigned char* buf, size_t size)
{
	unsigned long sum = 0;
	for (size_t i = 0; i < size; ++i)
		sum += *buf++;
	return sum;
}

unsigned long FileImpl::LogInfo()
{
	std::wstringstream wss;
	std::wstring spc(L"\t");
	wss.write(m_filePath.c_str(), m_filePath.size());
	wss.write(spc.c_str(), spc.size());
	wss.write(m_fileDate.c_str(), m_fileDate.size());
	wss.write(spc.c_str(), spc.size());
	wss.write(m_fileSize.c_str(), m_fileSize.size());
	wss.write(spc.c_str(), spc.size());

	//Sum count thread pool test
	//TODO: opend file by parts and feed to computeSumBuf

	//ThreadPool check
	//ThreadPool tp(ThreadPool::THREAD_NUMBER);
	//unsigned char a[3] = { 0, 1, 2 };
	//tp.enqueue(&FileImpl::computeSumBuf, this, &a[0], 3);
	//tp.GetResult();

	//computeSum is way too slow
	wss << std::to_wstring(computeSum());
	Logger::GetInstance()->LogIt(wss.str());

	return 0;
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

	ULONGLONG fileSize = (static_cast<ULONGLONG>(lpFileInformation.nFileSizeHigh) <<
                      sizeof(lpFileInformation.nFileSizeLow) * 8) |
                     lpFileInformation.nFileSizeLow;
	ULONGLONG fileSizeKB = (fileSize / 1024) == 0 ? 1 : fileSize / 1024;
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
	res = GetDateFormat(LOCALE_USER_DEFAULT, NULL, &lSystemTime, L"dd'.'MM'.'yyyy", &date[0], 20);
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

