#ifndef W32CPP_WIN_WIN_HPP
#define W32CPP_WIN_WIN_HPP

#include <w32cpp/win/err.hpp>

namespace W32CPP {
	class Win {
	public:
		static LRESULT defWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static HRESULT initialize(WNDPROC lpfnWndProc, HWND* oHWND);
		static bool    dispatchGetMessage();
		static bool    dispatchPeekMessage();
	};
}

#endif
