#include <w32cpp/ebest/xasession.hpp>
#include <w32cpp/win/win.hpp>

#import "C:\eBEST\xingAPI\XA_Session.dll" named_guids

using namespace W32CPP;

constexpr auto DISPID_LOGIN = 1;
constexpr auto DISPID_LOGOUT = 2;
constexpr auto DISPID_DISCONNECT = 3;

enum XASESSION_SESSION_STATUS {
	XASESSION_SESSION_DISCONNECT,
	XASESSION_SESSION_LOGIN_SUCCEEDED,
	XASESSION_SESSION_LOGIN_FAILED,
};

class XASession::Impl : XA_SESSIONLib::_IXASessionEvents {
private:
	XA_SESSIONLib::IXASessionPtr __com;
	XASESSION_SESSION_STATUS __status;

	void __disconnect()
	{
		HRESULT hr = __com->DisconnectServer();
		__status = XASESSION_SESSION_DISCONNECT;
	}

public:
	Impl() :
		__com(),
		__status(XASESSION_SESSION_DISCONNECT)
	{
	}

	~Impl()
	{
	}

	HRESULT initialize()
	{
		HRESULT hr;
		hr = __com.CreateInstance(__uuidof(XA_SESSIONLib::XASession), nullptr, CLSCTX_INPROC_SERVER);
		if (FAILED(hr)) {
			return hr;
		}

		IConnectionPointContainerPtr pCPC;
		hr = __com->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
		if (FAILED(hr)) {
			return hr;
		}

		IConnectionPointPtr pCP;
		hr = pCPC->FindConnectionPoint(__uuidof(XA_SESSIONLib::_IXASessionEvents), &pCP);
		if (FAILED(hr)) {
			return hr;
		}

		DWORD dwCookie;
		hr = pCP->Advise((IUnknown*)this, &dwCookie);
		if (FAILED(hr)) {
			return hr;
		}

		return S_OK;
	}

	HRESULT OnLogin(_bstr_t szCode, _bstr_t szMsg)
	{
		if (strcmp((const char*)szCode, "0000") == 0) {
			__status = XASESSION_SESSION_LOGIN_SUCCEEDED;
		}
		else {
			__status = XASESSION_SESSION_LOGIN_FAILED;
		}
		// TODO: CALLBACK.OnLogin
		return S_OK;
	}

	HRESULT OnLogout()
	{
		__disconnect();
		// TODO: CALLBACK.OnDisconnect
		return S_OK;
	}

	HRESULT OnDisconnect()
	{
		__disconnect();
		// TODO: CALLBACK.OnDisconnect
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT* pctinfo)
	{
		return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo)
	{
		return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId)
	{
		return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE Invoke(
		DISPID dispIdMember,
		REFIID riid,
		LCID lcid,
		WORD wFlags,
		DISPPARAMS* pDispParams,
		VARIANT* pVarResult,
		EXCEPINFO* pExcepInfo,
		UINT* puArgErr
	)
	{
		VARIANTARG* varg = pDispParams->rgvarg;
		switch (dispIdMember)
		{
		case DISPID_LOGIN:
			return OnLogin(varg[1].bstrVal, varg[0].bstrVal);
		case DISPID_LOGOUT:
			return OnLogout();
		case DISPID_DISCONNECT:
			return OnDisconnect();
		default:
			return E_NOTIMPL;
		}
	}

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID* ppvObj)
	{
		if (ppvObj == nullptr)
			return E_INVALIDARG;
		*ppvObj = nullptr;
		if (riid == __uuidof(IUnknown) || riid == __uuidof(IDispatch) || riid == __uuidof(XA_SESSIONLib::_IXASessionEvents))
		{
			*ppvObj = this;
			AddRef();
			return NOERROR;
		}
		return E_NOINTERFACE;
	}

	ULONG STDMETHODCALLTYPE AddRef()
	{
		return 1;
	}

	ULONG STDMETHODCALLTYPE Release()
	{
		return 1;
	}

	HRESULT login(
		_bstr_t szID,
		_bstr_t szPwd,
		_bstr_t szCertPwd,
		long nServerType,
		_bstr_t szServerIP,
		long nServerPort
	)
	{
		BOOL bShowCertErrDlg = FALSE;
		__disconnect();
		if (__com->ConnectServer(szServerIP, nServerPort) == FALSE) {
			int nErrorCode = __com->GetLastError();
			return ERR_MAKE(EBEST_FACILITY_CONNECT, -nErrorCode);
		}
		if (__com->Login(szID, szPwd, szCertPwd, nServerType, bShowCertErrDlg) == FALSE) {
			int nErrorCode = __com->GetLastError();
			return ERR_MAKE(EBEST_FACILITY_LOGIN, -nErrorCode);
		}

		while (__status == XASESSION_SESSION_DISCONNECT) {
			Win::dispatchPeekMessage();
		}

		if (__status == XASESSION_SESSION_LOGIN_SUCCEEDED) {
			return S_OK;
		}
		return ERR_MAKE(EBEST_FACILITY_LOGIN, ERR_CODE_ARGS);
	}
};

XASession::XASession() :
	__impl()
{
}

XASession::~XASession()
{
}

HRESULT XASession::initialize()
{
	__impl = std::make_unique<Impl>();
	if (__impl) {
		return __impl->initialize();
	}
	return ERR_MAKE(EBEST_FACILITY_IMPL, ERR_CODE_ARGS);
}

HRESULT XASession::login(LPCSTR szID, LPCSTR szPwd, LPCSTR szCertPwd, long nServerType, LPCSTR szServer, long nServerPort)
{
	if (__impl) {
		return __impl->login(szID, szPwd, szCertPwd, nServerType, szServer, nServerPort);
	}
	return ERR_MAKE(EBEST_FACILITY_IMPL, ERR_CODE_ARGS);
}
