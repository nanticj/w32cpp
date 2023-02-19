#ifndef W32CPP_WIN_DLL_HPP
#define W32CPP_WIN_DLL_HPP

#include <w32cpp/win/err.hpp>

namespace W32CPP {
	class DLL {
	private:
		HMODULE __hModule;

	public:
		DLL();
		~DLL();
		HRESULT initialize(LPCWSTR lpLibFileName);
		FARPROC getProcAddress(LPCSTR lpProcName);
	};
}

#endif
