#pragma once

#include <Windows.h>
#include <ShlObj.h>
#include <vector>
#include <memory>
#include <File.h>

static const wchar_t* kContextMenuItemName = L"&Log it!";

class ContextMenuComponent : public IShellExtInit, public IContextMenu
{
public:
	// IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void **ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

	// IShellExtInit
    IFACEMETHODIMP Initialize(LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hKeyProgID);

	// IContextMenu
	IFACEMETHODIMP QueryContextMenu(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);
	IFACEMETHODIMP InvokeCommand(CMINVOKECOMMANDINFO* pici);
	IFACEMETHODIMP GetCommandString(UINT_PTR, UINT, UINT*, CHAR*, UINT);

	ContextMenuComponent();

protected:
	~ContextMenuComponent();

private:
    // Reference count of component.
    long m_cRef;

	std::vector<std::wstring> m_files;

	int HandleLogCommand();
};