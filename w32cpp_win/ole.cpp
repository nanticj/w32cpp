#include <w32cpp/win/ole.hpp>

using namespace W32CPP;

OLE::OLE() :
	__init(CO_E_NOTINITIALIZED)
{
}

OLE::~OLE()
{
	if (SUCCEEDED(__init)) {
		OleUninitialize();
	}
}

HRESULT OLE::initialize()
{
	__init = OleInitialize(nullptr);
	return __init;
}
