#include <w32cpp/win/dll.hpp>

using namespace W32CPP;

DLL::DLL() :
	__hModule(nullptr)
{
}

DLL::~DLL()
{
	if (__hModule != nullptr) {
		FreeLibrary(__hModule);
	}
}

HRESULT DLL::initialize(LPCWSTR lpLibFileName)
{
	if (lpLibFileName == nullptr) {
		return ERR_MAKE(ERR_FACILITY_DLL, ERR_CODE_ARGS);
	}

	__hModule = LoadLibraryW(lpLibFileName);
	if (__hModule == nullptr) {
		return ERR_MAKE(ERR_FACILITY_DLL, ERR_CODE_INTERNAL1);
	}

	return S_OK;
}

FARPROC DLL::getProcAddress(LPCSTR lpProcName)
{
	return GetProcAddress(__hModule, lpProcName);
}
