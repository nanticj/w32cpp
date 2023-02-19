#ifndef W32CPP_EBEST_XASESSION_HPP
#define W32CPP_EBEST_XASESSION_HPP

#include <w32cpp/ebest/err.hpp>
#include <memory>

namespace W32CPP {
	class XASession
	{
	private:
		class Impl;
		std::unique_ptr<Impl> __impl;

	public:
		XASession();
		~XASession();
		HRESULT initialize();
		HRESULT login(
			LPCSTR szID,
			LPCSTR szPwd,
			LPCSTR szCertPwd,
			long nServerType = 0,
			LPCSTR szServer = "hts.ebestsec.co.kr",
			long nServerPort = 20001
		);
	};
}

#endif
