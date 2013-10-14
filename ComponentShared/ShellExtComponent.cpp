#include "ShellExtComponent.h"
#include <Windows.h>
#include <Shlwapi.h>
#include <strsafe.h>
#include <string>
#include <fstream>
#include <iostream>

#include "Logger.h"
#include <File.h>
#include "ThreadPool.h"

#pragma comment(lib, "shlwapi.lib")

extern HINSTANCE g_hInst;
extern long g_cDllRef;

ShellExtComponent::ShellExtComponent() : m_cRef(1)
{
    InterlockedIncrement(&g_cDllRef);
}

ShellExtComponent::~ShellExtComponent()
{
    InterlockedDecrement(&g_cDllRef);
}

#pragma region IUnknown

// Query to the interface the component supported.
IFACEMETHODIMP ShellExtComponent::QueryInterface(REFIID riid, void **ppv)
{
    static const QITAB qit[] = 
    {
        QITABENT(ShellExtComponent, IShellExtInit), 
		QITABENT(ShellExtComponent, IContextMenu),
        { 0 },
    };
    return QISearch(this, qit, riid, ppv);
}

// Increase the reference count for an interface on an object.
IFACEMETHODIMP_(ULONG) ShellExtComponent::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

// Decrease the reference count for an interface on an object.
IFACEMETHODIMP_(ULONG) ShellExtComponent::Release()
{
    ULONG cRef = InterlockedDecrement(&m_cRef);
    if (0 == cRef)
    {
        delete this;
    }

    return cRef;
}

#pragma endregion

#pragma region IShellExtInit

// Initialize the context menu extension.
IFACEMETHODIMP ShellExtComponent::Initialize(
    LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hKeyProgID)
{
    if (NULL == pDataObj)
    {
        return E_INVALIDARG;
    }

    HRESULT hr = E_FAIL;

    FORMATETC fe = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
    STGMEDIUM stm;

    // The pDataObj pointer contains the objects being acted upon. In this 
    // example, we get an HDROP handle for enumerating the selected files and 
    // folders.
    if (SUCCEEDED(pDataObj->GetData(&fe, &stm)))
    {
        // Get an HDROP handle.
        HDROP hDrop = static_cast<HDROP>(GlobalLock(stm.hGlobal));
        if (hDrop != NULL)
        {
            // Determine how many files are involved in this operation. This 
            // code sample displays the custom context menu item when only 
            // one file is selected. 
            UINT nFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
			for (UINT i = 0; i < nFiles;  i++)
			{
				try
				{
					std::shared_ptr<wchar_t> filePath(new wchar_t[MAX_PATH], deleter<wchar_t>());
					DragQueryFile(hDrop, i, filePath.get(), 
									MAX_PATH);

					m_filesVec.push_back(filePath);
				}
				catch (const std::exception& e)
				{
					std::cerr << "ShellExtComponent::Initialize " << e.what() << std::endl;
				}

			}
			hr = S_OK;

            GlobalUnlock(stm.hGlobal);
        }

        ReleaseStgMedium(&stm);
    }

    // If any value other than S_OK is returned from the method, the property 
    // sheet is not displayed.
    return hr;
}

#pragma endregion

#pragma region IContextMenu
#define IDM_DISPLAY 0

IFACEMETHODIMP ShellExtComponent::QueryContextMenu( 
            _In_  HMENU hMenu,
            _In_  UINT indexMenu,
            _In_  UINT idCmdFirst,
            _In_  UINT idCmdLast,
            _In_  UINT uFlags)
{
    if(!(CMF_DEFAULTONLY & uFlags))
    {
        InsertMenu(hMenu, 
                   indexMenu, 
                   MF_STRING | MF_BYPOSITION, 
                   idCmdFirst + IDM_DISPLAY, 
                   L"&Log it!");

        return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(IDM_DISPLAY + 1));
    }

    return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(0));
}

IFACEMETHODIMP ShellExtComponent::InvokeCommand( 
            CMINVOKECOMMANDINFO *pici)
{
	ThreadPool tp(ThreadPool::THREAD_NUMBER);
	for(auto& it : m_filesVec)
	{
		//!!!MEMLEAK

		//In test purposes
		MessageBox(NULL, it.get(), NULL, 0);
		File* f = new File(it.get());
		tp.enqueue(&File::LogInfo, f);
	}

	return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(0));
}

IFACEMETHODIMP ShellExtComponent::GetCommandString( 
			UINT_PTR idCmd,
			UINT uType,
			UINT *pReserved,
			CHAR *pszName,
			UINT cchMax)
{

	return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(0));
}

#pragma endregion