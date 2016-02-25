#include <sstream>

#include <MainApp.h>
#include <File.h>
#include <Logger.h>

MainApp::MainApp(const FileVector& files)
	: m_files(std::move(files))
	, m_threadPool()
{
	
}

void MainApp::Run()
{
	for (const auto& path : m_files)
	{
		TaskQueue::type function_object(
			[&]()
		{
			File file(path);
			unsigned long long sum = file.GetCheckSum();
			std::wstringstream wss;
			std::wstring spc = L"\t";
			wss << file.Path() << spc;
			wss << file.SizeKb() << spc;
			wss << file.Date() << spc;
			wss << sum << std::endl;

			Logger::GetInstance()->LogIt(wss.str());
		});

		m_threadPool.Enqueue(function_object);
	}

	m_threadPool.Run();
}