#include "ContextMenuComponent.h"
#include <Windows.h>
#include <Shlwapi.h>
#include <strsafe.h>
#include <string>
#include <fstream>
#include <iostream>

#include <MainApp.h>
#include <ThreadPool.h>

#pragma comment(lib, "shlwapi.lib")

extern long g_cDllRef;

ContextMenuComponent::ContextMenuComponent() : m_cRef(1)
{
    InterlockedIncrement(&g_cDllRef);
}

ContextMenuComponent::~ContextMenuComponent()
{
	m_files.clear();
    InterlockedDecrement(&g_cDllRef);
}

#pragma region IUnknown

// Query to the interface the component supported.
IFACEMETHODIMP ContextMenuComponent::QueryInterface(REFIID riid, void **ppv)
{
    static const QITAB qit[] = 
    {
        QITABENT(ContextMenuComponent, IShellExtInit), 
		QITABENT(ContextMenuComponent, IContextMenu),
        { 0 },
    };
    return QISearch(this, qit, riid, ppv);
}

// Increase the reference count for an interface on an object.
IFACEMETHODIMP_(ULONG) ContextMenuComponent::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

// Decrease the reference count for an interface on an object.
IFACEMETHODIMP_(ULONG) ContextMenuComponent::Release()
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
IFACEMETHODIMP ContextMenuComponent::Initialize(
    LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hKeyProgID)
{
    if (NULL == pDataObj)
    {
        return E_INVALIDARG;
    }

    HRESULT hr = E_FAIL;

    FORMATETC fe = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
    STGMEDIUM stm;


    if (SUCCEEDED(pDataObj->GetData(&fe, &stm)))
    {
        HDROP hDrop = static_cast<HDROP>(GlobalLock(stm.hGlobal));
        if (hDrop != NULL)
        {

            UINT nFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
			for (UINT i = 0; i < nFiles;  i++)
			{
				wchar_t tmpPath[MAX_PATH + 1] = { '\0' };
				if (0 != DragQueryFile(hDrop, i, tmpPath, ARRAYSIZE(tmpPath)))
				{
					//Skip directories
					std::wstring curPath(tmpPath);
					if (!PathIsDirectoryW(curPath.c_str()))
					{
						m_files.push_back(std::wstring(tmpPath));
						hr = S_OK;
					}
				}
			}

            GlobalUnlock(stm.hGlobal);
        }

        ReleaseStgMedium(&stm);
    }

    return hr;
}

#pragma endregion

#pragma region IContextMenu
#define IDM_DISPLAY 0

IFACEMETHODIMP ContextMenuComponent::QueryContextMenu( 
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
				   kContextMenuItemName);

        return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(IDM_DISPLAY + 1));
    }

	return S_OK;
}

IFACEMETHODIMP ContextMenuComponent::InvokeCommand( 
            CMINVOKECOMMANDINFO *pici)
{
	//We have single command to execute
	HandleLogCommand();

	return S_OK;
}

IFACEMETHODIMP ContextMenuComponent::GetCommandString( 
			UINT_PTR idCmd,
			UINT uType,
			UINT *pReserved,
			CHAR *pszName,
			UINT cchMax)
{

	return S_OK;
}

int ContextMenuComponent::HandleLogCommand()
{
	MainApp app(m_files);
	app.Run();
	return 0;
}

#pragma endregion