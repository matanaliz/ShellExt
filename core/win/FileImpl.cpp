#include <sstream>
#include <fstream>
#include <Logger.h>
#include <ThreadPool.h>
#include "FileImpl.h"


FileImpl::FileImpl(const std::wstring& path) 
	: m_path(path)
	, m_date(L"")
	, m_size(0)
	, m_sizeKb(0)
{
}

FileImpl::~FileImpl()
{
}

unsigned long long FileImpl::computeSum()
{
	unsigned long long sum = 0;
	std::ifstream fstr;
	try
	{
		fstr.open(m_path.c_str(), std::fstream::in | std::fstream::binary);
		if (fstr.is_open())
		{
			while (!fstr.eof() && fstr.good())
			{
				// Possible overflow. Change checksum to some other.
				char c;
				fstr.get(c);
				sum += static_cast<unsigned long>(c);
			}
			fstr.close();
		}
	}
	catch (...)
	{
		// TODO Log exceptions with logger all over the project.
	}

	return sum;
}

//unsigned long FileImpl::LogInfo()
//{
//	std::wstringstream wss;
//	std::wstring spc(L"\t");
//	wss.write(m_filePath.c_str(), m_filePath.size());
//	wss.write(spc.c_str(), spc.size());
//	wss.write(m_fileDate.c_str(), m_fileDate.size());
//	wss.write(spc.c_str(), spc.size());
//	wss.write(m_fileSize.c_str(), m_fileSize.size());
//	wss.write(spc.c_str(), spc.size());
//
//	//Sum count thread pool test
//	//TODO: opend file by parts and feed to computeSumBuf
//
//	//ThreadPool check
//	//ThreadPool tp(ThreadPool::THREAD_NUMBER);
//	//unsigned char a[3] = { 0, 1, 2 };
//	//tp.enqueue(&FileImpl::computeSumBuf, this, &a[0], 3);
//	//tp.GetResult();
//
//	//computeSum is way too slow
//	wss << std::to_wstring(computeSum());
//	Logger::GetInstance()->LogIt(wss.str());
//
//	return 0;
//}

const std::wstring& FileImpl::Date()
{
	if (m_date.empty())
	{
		WIN32_FILE_ATTRIBUTE_DATA fileInformation;
		if (0 != GetFileAttributesEx(
			m_path.c_str(),
			GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard,
			&fileInformation))
		{
			SYSTEMTIME lSystemTime;
			FILETIME ftCreationTime = fileInformation.ftCreationTime;
			if (0 != FileTimeToSystemTime(&ftCreationTime, &lSystemTime))
			{
				wchar_t tmpDate[MAX_PATH + 1] = { '\0' };
				if (0 != GetDateFormat(
					LOCALE_USER_DEFAULT,
					NULL,
					&lSystemTime,
					L"dd'.'MM'.'yyyy",
					tmpDate, ARRAYSIZE(tmpDate)))
				{
					std::wstringstream wss;
					wss << tmpDate;
					wchar_t tmpTime[MAX_PATH + 1] = { '\0' };
					if (0 != GetTimeFormatEx(
						LOCALE_NAME_USER_DEFAULT, 
						TIME_FORCE24HOURFORMAT, 
						&lSystemTime, 
						L"HH:mm:ss", 
						tmpTime, 
						ARRAYSIZE(tmpTime)))
					{
						wss << tmpTime;
					}
					m_date = wss.str();
				}
			}
		}
	}

	return m_date;
}

const std::wstring& FileImpl::Path()
{
	return m_path;
}

const unsigned long long FileImpl::Size()
{
	if (0 == m_size)
	{
		WIN32_FILE_ATTRIBUTE_DATA fileInformation;
		if (0 != GetFileAttributesEx(m_path.c_str(),
			GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard,
			&fileInformation))
		{
			m_size = (static_cast<ULONGLONG>(fileInformation.nFileSizeHigh) <<
				sizeof(fileInformation.nFileSizeLow) * 8) |
				fileInformation.nFileSizeLow;
			m_sizeKb = (m_size / 1024) == 0 ? 1 : m_size / 1024;
		}
	}
	return m_size;
}

const unsigned long long FileImpl::SizeKb()
{
	if (0 == m_size)
		Size();

	return m_sizeKb;
}