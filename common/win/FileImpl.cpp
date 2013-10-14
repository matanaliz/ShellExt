#include "FileImpl.h"
//Change
#include "../../ComponentShared/Logger.h"


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
		//prepareInfo();
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
	Logger::GetInstance()->LogIt(m_filePath);
}

void FileImpl::prepareInfo()
{
	//TODO: check for result
	WIN32_FILE_ATTRIBUTE_DATA lpFileInformation;
	GetFileAttributesEx(m_filePath.c_str(), GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &lpFileInformation);
	
	SYSTEMTIME lSystemTime;
	FileTimeToSystemTime(&lpFileInformation.ftCreationTime, &lSystemTime);

	//int size = GetTimeFormatEx(LOCALE_NAME_INVARIANT, TIME_FORCE24HOURFORMAT, &lSystemTime, NULL, NULL, 0);

	m_fileDate.resize(256, 0);
	GetDateFormat(LOCALE_INVARIANT, DATE_SHORTDATE, &lSystemTime, L"MMM dd yy", &(m_fileDate[0]), 256);
}

