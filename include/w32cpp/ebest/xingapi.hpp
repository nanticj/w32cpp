#ifndef W32CPP_EBEST_XINGAPI_HPP
#define W32CPP_EBEST_XINGAPI_HPP

#include <w32cpp/ebest/err.hpp>

namespace W32CPP {
	class XingAPI {
	private:
		class Impl;

		XingAPI();
		~XingAPI();
		XingAPI(const XingAPI&) = delete;
		XingAPI& operator=(const XingAPI&) = delete;

	public:
		static XingAPI& getInstance();

		HRESULT initialize();
		HRESULT login(
			LPCSTR szID,
			LPCSTR szPwd,
			LPCSTR szCertPwd,
			int nServerType = 0,
			LPCSTR szServer = "hts.ebestsec.co.kr",
			int nServerPort = 20001
		);
	};
}

#endif
