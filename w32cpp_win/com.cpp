#include <w32cpp/win/com.hpp>

using namespace W32CPP;

COM::COM() :
	__init(CO_E_NOTINITIALIZED)
{
}

COM::~COM()
{
	if (SUCCEEDED(__init)) {
		CoUninitialize();
	}
}

HRESULT COM::initialize(DWORD dwCoInit)
{
	__init = CoInitializeEx(nullptr, dwCoInit);
	return __init;
}
