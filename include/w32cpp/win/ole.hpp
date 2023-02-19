#ifndef W32CPP_WIN_OLE_HPP
#define W32CPP_WIN_OLE_HPP

#include <w32cpp/win/err.hpp>

namespace W32CPP {
	class OLE {
	private:
		HRESULT __init;

	public:
		OLE();
		~OLE();
		HRESULT initialize();
	};
}

#endif
