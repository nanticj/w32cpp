#ifndef W32CPP_WIN_COM_HPP
#define W32CPP_WIN_COM_HPP

#include <w32cpp/win/err.hpp>

namespace W32CPP {
	class COM {
	private:
		HRESULT __init;

	public:
		COM();
		~COM();
		HRESULT initialize(DWORD dwCoInit = COINIT_APARTMENTTHREADED);
	};
}

#endif
