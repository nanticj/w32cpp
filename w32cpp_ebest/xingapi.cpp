#include <w32cpp/ebest/packet.hpp>
#include <w32cpp/ebest/xingapi.hpp>
#include <w32cpp/win/dll.hpp>
#include <w32cpp/win/win.hpp>

using namespace W32CPP;

typedef BOOL(WINAPI* FP_CONNECT)(HWND, LPCSTR, int, int, int, int);
typedef BOOL(WINAPI* FP_ISCONNECTED)();
typedef BOOL(WINAPI* FP_DISCONNECT)();
typedef BOOL(WINAPI* FP_LOGIN)(HWND, LPCSTR, LPCSTR, LPCSTR, int, BOOL);
typedef int (WINAPI* FP_REQUEST)(HWND, LPCSTR, LPCVOID, int, BOOL, LPCSTR, int);
typedef void(WINAPI* FP_RELEASEREQUESTDATA)(int);
typedef void(WINAPI* FP_RELEASEMESSAGEDATA)(LPARAM);
typedef BOOL(WINAPI* FP_ADVISEREALDATA)(HWND, LPCSTR, LPCVOID, int);
typedef BOOL(WINAPI* FP_UNADVISEREALDATA)(HWND, LPCSTR, LPCVOID, int);
typedef BOOL(WINAPI* FP_UNADVISEWINDOW)(HWND);
typedef int (WINAPI* FP_GETLASTERROR)();

constexpr int XM_DISCONNECT = WM_USER + 1;
constexpr int XM_RECEIVE_DATA = WM_USER + 3;
constexpr int XM_RECEIVE_REAL_DATA = WM_USER + 4;
constexpr int XM_LOGIN = WM_USER + 5;
constexpr int XM_LOGOUT = WM_USER + 6;

constexpr int REQUEST_DATA = 1;
constexpr int MESSAGE_DATA = 2;
constexpr int SYSTEM_ERROR_DATA = 3;
constexpr int RELEASE_DATA = 4;

enum XINGAPI_SESSION_STATUS {
	XINGAPI_SESSION_DISCONNECT,
	XINGAPI_SESSION_LOGIN_SUCCEEDED,
	XINGAPI_SESSION_LOGIN_FAILED,
};

class XingAPI::Impl {
private:
	HWND __hWnd;
	DLL  __dll;
	FP_CONNECT __ETK_Connect;
	FP_ISCONNECTED __ETK_IsConnected;
	FP_DISCONNECT __ETK_Disconnect;
	FP_LOGIN __ETK_Login;
	FP_REQUEST __ETK_Request;
	FP_RELEASEREQUESTDATA __ETK_ReleaseRequestData;
	FP_RELEASEMESSAGEDATA __ETK_ReleaseMessageData;
	FP_ADVISEREALDATA __ETK_AdviseRealData;
	FP_UNADVISEREALDATA __ETK_UnadviseRealData;
	FP_UNADVISEWINDOW __ETK_UnadviseWindow;
	FP_GETLASTERROR __ETK_GetLastError;
	XINGAPI_SESSION_STATUS __status;

	static LRESULT CALLBACK __wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		Impl& impl = getInstance();
		switch (message) {
		case XM_DISCONNECT:
			impl.__onDisconnect(wParam, lParam);
			break;
		case XM_RECEIVE_DATA:
			impl.__onReceiveData(wParam, lParam);
			break;
		case XM_RECEIVE_REAL_DATA:
			impl.__onReceiveRealData(wParam, lParam);
			break;
		case XM_LOGIN:
			impl.__onLogin(wParam, lParam);
			break;
		case XM_LOGOUT:
			impl.__onLogout(wParam, lParam);
			break;
		default:
			return Win::defWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	Impl() :
		__hWnd(nullptr),
		__dll(),
		__ETK_Connect(nullptr),
		__ETK_IsConnected(nullptr),
		__ETK_Disconnect(nullptr),
		__ETK_Login(nullptr),
		__ETK_Request(nullptr),
		__ETK_ReleaseRequestData(nullptr),
		__ETK_ReleaseMessageData(nullptr),
		__ETK_AdviseRealData(nullptr),
		__ETK_UnadviseRealData(nullptr),
		__ETK_UnadviseWindow(nullptr),
		__ETK_GetLastError(nullptr),
		__status(XINGAPI_SESSION_DISCONNECT)
	{
	}

	~Impl()
	{
	}

	Impl(const Impl&) = delete;
	Impl& operator=(const Impl&) = delete;

	void __onDisconnect(WPARAM wParam, LPARAM lParam)
	{
		__disconnect();
		// TODO: CALLBACK.OnDisconnect
	}

	void __onReceiveData(WPARAM wParam, LPARAM lParam)
	{
		switch (wParam) {
		case REQUEST_DATA:
			__onReceiveRequestData(wParam, lParam);
			break;
		case MESSAGE_DATA:
			__onReceiveMessageData(wParam, lParam);
			break;
		case SYSTEM_ERROR_DATA:
			__onReceiveSystemErrorData(wParam, lParam);
			break;
		case RELEASE_DATA:
			__onReceiveReleaseData(wParam, lParam);
			break;
		}
	}

	void __onReceiveRequestData(WPARAM wParam, LPARAM lParam)
	{
		LPRECV_PACKET p = (LPRECV_PACKET)lParam;
		// TODO: CALLBACK.onReceiveRequestData
	}

	void __onReceiveMessageData(WPARAM wParam, LPARAM lParam)
	{
		LPMSG_PACKET p = (LPMSG_PACKET)lParam;
		__ETK_ReleaseMessageData(lParam);
	}

	void __onReceiveSystemErrorData(WPARAM wParam, LPARAM lParam)
	{
		LPMSG_PACKET p = (LPMSG_PACKET)lParam;
		// TODO: CALLBACK.onReceiveRequestData
		__ETK_ReleaseMessageData(lParam);
	}

	void __onReceiveReleaseData(WPARAM wParam, LPARAM lParam)
	{
		int nRqID = (int)lParam;
		__ETK_ReleaseRequestData((int)lParam);
	}

	void __onReceiveRealData(WPARAM wParam, LPARAM lParam)
	{
		LPRECV_REAL_PACKET p = (LPRECV_REAL_PACKET)lParam;
		// TODO: CALLBACK.onReceiveRealData
	}

	void __onLogin(WPARAM wParam, LPARAM lParam)
	{
		LPCSTR pszCode = (LPCSTR)wParam;
		LPCSTR pszMsg = (LPCSTR)lParam;
		if (strcmp(pszCode, "0000") == 0) {
			__status = XINGAPI_SESSION_LOGIN_SUCCEEDED;
		}
		else {
			__status = XINGAPI_SESSION_LOGIN_FAILED;
		}
		// TODO: CALLBACK.OnLogin
	}

	void __onLogout(WPARAM wParam, LPARAM lParam)
	{
		__disconnect();
		// TODO: CALLBACK.OnDisconnect
	}

	void __disconnect()
	{
		__ETK_Disconnect();
		__status = XINGAPI_SESSION_DISCONNECT;
	}

public:
	static Impl& getInstance()
	{
		static Impl instance;
		return instance;
	}

	HRESULT initialize()
	{
		HRESULT hr = __dll.initialize(L"C:\\eBEST\\xingAPI\\xingAPI.dll");
		if (FAILED(hr)) {
			return hr;
		}

		__ETK_Connect = (FP_CONNECT)__dll.getProcAddress("ETK_Connect");
		__ETK_IsConnected = (FP_ISCONNECTED)__dll.getProcAddress("ETK_IsConnected");
		__ETK_Disconnect = (FP_DISCONNECT)__dll.getProcAddress("ETK_Disconnect");
		__ETK_Login = (FP_LOGIN)__dll.getProcAddress("ETK_Login");
		__ETK_Request = (FP_REQUEST)__dll.getProcAddress("ETK_Request");
		__ETK_ReleaseRequestData = (FP_RELEASEREQUESTDATA)__dll.getProcAddress("ETK_ReleaseRequestData");
		__ETK_ReleaseMessageData = (FP_RELEASEMESSAGEDATA)__dll.getProcAddress("ETK_ReleaseMessageData");
		__ETK_AdviseRealData = (FP_ADVISEREALDATA)__dll.getProcAddress("ETK_AdviseRealData");
		__ETK_UnadviseRealData = (FP_UNADVISEREALDATA)__dll.getProcAddress("ETK_UnadviseRealData");
		__ETK_UnadviseWindow = (FP_UNADVISEWINDOW)__dll.getProcAddress("ETK_UnadviseWindow");
		__ETK_GetLastError = (FP_GETLASTERROR)__dll.getProcAddress("ETK_GetLastError");
		if (__ETK_Connect == nullptr ||
			__ETK_IsConnected == nullptr ||
			__ETK_Disconnect == nullptr ||
			__ETK_Login == nullptr ||
			__ETK_Request == nullptr ||
			__ETK_ReleaseRequestData == nullptr ||
			__ETK_ReleaseMessageData == nullptr ||
			__ETK_AdviseRealData == nullptr ||
			__ETK_UnadviseRealData == nullptr ||
			__ETK_UnadviseWindow == nullptr ||
			__ETK_GetLastError == nullptr)
		{
			return ERR_MAKE(EBEST_FACILITY_IMPL, ERR_CODE_ARGS);
		}

		hr = Win::initialize(__wndProc, &__hWnd);
		if (FAILED(hr)) {
			return hr;
		}

		return S_OK;
	}

	HRESULT login(
		LPCSTR szID,
		LPCSTR szPwd,
		LPCSTR szCertPwd,
		int nServerType,
		LPCSTR szServer,
		int nServerPort
	)
	{
		int nStartMsgID = WM_USER;
		int nConnectTimeOut = -1;
		int nSendPacketSize = -1;
		BOOL bShowCertErrDlg = FALSE;

		__disconnect();
		if (__ETK_Connect(__hWnd, szServer, nServerPort, nStartMsgID, nConnectTimeOut, nSendPacketSize) == FALSE) {
			int nErrorCode = __ETK_GetLastError();
			return ERR_MAKE(EBEST_FACILITY_CONNECT, -nErrorCode);
		}
		if (__ETK_Login(__hWnd, szID, szPwd, szCertPwd, nServerType, bShowCertErrDlg) == FALSE) {
			int nErrorCode = __ETK_GetLastError();
			return ERR_MAKE(EBEST_FACILITY_LOGIN, -nErrorCode);
		}

		while (__status == XINGAPI_SESSION_DISCONNECT) {
			Win::dispatchPeekMessage();
		}

		if (__status == XINGAPI_SESSION_LOGIN_SUCCEEDED) {
			return S_OK;
		}
		return ERR_MAKE(EBEST_FACILITY_LOGIN, ERR_CODE_ARGS);
	}
};

XingAPI::XingAPI()
{
}

XingAPI::~XingAPI()
{
}

XingAPI& XingAPI::getInstance()
{
	static XingAPI instance;
	return instance;
}

HRESULT XingAPI::initialize()
{
	Impl& impl = Impl::getInstance();
	return impl.initialize();
}

HRESULT XingAPI::login(
	LPCSTR szID,
	LPCSTR szPwd,
	LPCSTR szCertPwd,
	int nServerType,
	LPCSTR szServer,
	int nServerPort
)
{
	Impl& impl = Impl::getInstance();
	return impl.login(
		szID,
		szPwd,
		szCertPwd,
		nServerType,
		szServer,
		nServerPort
	);
}
