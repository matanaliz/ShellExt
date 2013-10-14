#pragma once

#include <Windows.h>
#include <ShlObj.h>
#include <vector>
#include <memory>

template <typename T> struct deleter
{
	void operator() (T* ptr)
	{
		delete[] ptr;
	}
};

class ShellExtComponent : public IShellExtInit, public IContextMenu
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

	ShellExtComponent();

protected:

	~ShellExtComponent();

private:
    // Reference count of component.
    long m_cRef;

	typedef std::shared_ptr<wchar_t> SharedArray;
	typedef std::vector<SharedArray> VectorShared;
	VectorShared m_filesVec;
};