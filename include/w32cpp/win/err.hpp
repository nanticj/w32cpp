#ifndef W32CPP_WIN_ERR_HPP
#define W32CPP_WIN_ERR_HPP

#include <Windows.h>

namespace W32CPP {

#define ERR_FIELD_SEVERITY ((HRESULT)0x80000000)
#define ERR_FIELD_CUSTOMER ((HRESULT)0x20000000)
#define ERR_FIELD_FACILITY ((HRESULT)0x07FF0000)
#define ERR_FIELD_CODE     ((HRESULT)0x0000FFFF)

#define ERR_MAKE(facility, code) \
( \
	ERR_FIELD_SEVERITY | \
	ERR_FIELD_CUSTOMER | \
	(ERR_FIELD_FACILITY & ((facility) << 16)) | \
	(ERR_FIELD_CODE & (code)) \
)

#define ERR_SEVERITY(hr)  ((ERR_FIELD_SEVERITY & (hr)) >> 31)
#define ERR_CUSTOMER(hr)  ((ERR_FIELD_CUSTOMER & (hr)) >> 29)
#define ERR_FACILITY(hr)  ((ERR_FIELD_FACILITY & (hr)) >> 16)
#define ERR_CODE(hr)      (ERR_FIELD_CODE & (hr))

#define ERR_FACILITY_WIN          (0)
#define ERR_FACILITY_DLL          (1)
#define ERR_FACILITY_COM          (2)
#define ERR_FACILITY_OLE          (3)

#define ERR_CODE_ARGS             (0)
#define ERR_CODE_INTERNAL1        (1)
#define ERR_CODE_INTERNAL2        (2)
#define ERR_CODE_INTERNAL3        (3)
#define ERR_CODE_INTERNAL4        (4)
#define ERR_CODE_INTERNAL5        (5)
#define ERR_CODE_INTERNAL6        (6)
#define ERR_CODE_INTERNAL7        (7)
#define ERR_CODE_INTERNAL8        (8)
#define ERR_CODE_INTERNAL9        (9)

}

#endif
