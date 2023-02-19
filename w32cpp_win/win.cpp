#include <w32cpp/win/win.hpp>

using namespace W32CPP;

BOOL CALLBACK __ctrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType) {
	case CTRL_C_EVENT:
	case CTRL_CLOSE_EVENT:
		PostMessageW(HWND_BROADCAST, WM_CLOSE, 0, 0);
		return TRUE;
	default:
		return FALSE;
	}
}

LRESULT Win::defWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hWnd, message, wParam, lParam);
	}
	return 0;
}

HRESULT Win::initialize(WNDPROC lpfnWndProc, HWND* oHWND)
{
	if (oHWND == nullptr) {
		return ERR_MAKE(ERR_FACILITY_WIN, ERR_CODE_ARGS);
	}
	*oHWND = nullptr;

	if (lpfnWndProc == nullptr) {
		return ERR_MAKE(ERR_FACILITY_WIN, ERR_CODE_ARGS);
	}

	if (SetConsoleCtrlHandler(__ctrlHandler, TRUE) == FALSE) {
		return ERR_MAKE(ERR_FACILITY_WIN, ERR_CODE_INTERNAL1);
	}

	HMODULE hInstance = GetModuleHandleW(nullptr);
	if (hInstance == nullptr) {
		return ERR_MAKE(ERR_FACILITY_WIN, ERR_CODE_INTERNAL2);
	}

	LPCWSTR lpszClassName = L"W32CPP_WIN_CLASS";
	LPCWSTR lpszWindowName = L"W32CPP_WIN_WINDOW";
	WNDCLASSEXW wcex{};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = 0;
	wcex.lpfnWndProc = lpfnWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = nullptr;
	wcex.hbrBackground = nullptr;
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = lpszClassName;
	wcex.hIconSm = nullptr;

	ATOM ret = RegisterClassExW(&wcex);
	if (ret == 0) {
		return ERR_MAKE(ERR_FACILITY_WIN, ERR_CODE_INTERNAL3);
	}

	HWND hWnd = CreateWindowExW(
		0,
		lpszClassName,
		lpszWindowName,
		0,
		0,
		0,
		0,
		0,
		HWND_MESSAGE,
		nullptr,
		hInstance,
		nullptr
	);
	if (hWnd == nullptr) {
		return ERR_MAKE(ERR_FACILITY_WIN, ERR_CODE_INTERNAL4);
	}

	*oHWND = hWnd;
	return S_OK;
}

bool Win::dispatchGetMessage()
{
	MSG msg;
	if (GetMessageW(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
		return true;
	}
	return false;
}

bool Win::dispatchPeekMessage()
{
	MSG msg;
	if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return false;
		}
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return true;
}
