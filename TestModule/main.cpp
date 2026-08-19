#include <atlbase.h>
#include <atlcom.h>
#include <Ems_i.h>
#include <Ems_i.c>
#include <wrl.h>

using namespace Microsoft::WRL;

class TestModule : public RuntimeClass<RuntimeClassFlags<ClassicCom>, IEmsExtensionModule>
{
public:
	TestModule() = default;

	STDMETHODIMP SetSite(_In_ IServiceProvider* pSP) noexcept override
	{
		if (!pSP)
		{
			provider_ = nullptr;
			return S_OK;
		}
		provider_ = pSP;
		return S_OK;
	}

	STDMETHODIMP QueryClose(_Outptr_ BOOL* pfCanClose) noexcept override
	{
		if (!pfCanClose) return E_POINTER;
		*pfCanClose = FALSE;
		return S_OK;
	}

	STDMETHODIMP Close() noexcept override
	{
		provider_ = nullptr;
		return S_OK;
	}

	STDMETHODIMP ModuleName(_Outptr_ BSTR* pBstrName) noexcept override
	{
		if (!pBstrName) return E_POINTER;
		ATL::CComBSTR bstr = L"System Explorer API Test Extension Module";
		return bstr.CopyTo(pBstrName);
	}

	STDMETHODIMP ModuleAuthor(_Outptr_ BSTR* pBstrAuthor) noexcept override
	{
		if (!pBstrAuthor) return E_POINTER;
		ATL::CComBSTR bstr = L"System Explorer Team";
		return bstr.CopyTo(pBstrAuthor);
	}

	STDMETHODIMP ModuleDescription(_Outptr_ BSTR* pBstrDescription) noexcept override
	{
		if (!pBstrDescription) return E_POINTER;
		ATL::CComBSTR bstr = L"Test module for verifying ALPC host connectivity.";
		return bstr.CopyTo(pBstrDescription);
	}

	STDMETHODIMP ModuleGuid(_Outptr_ GUID* pGuid) noexcept override
	{
		if (!pGuid) return E_POINTER;
		static const GUID TestModuleGuid =
		{ 0xa1b2c3d4, 0x1234, 0x5678, { 0x90, 0xab, 0xcd, 0xef, 0x12, 0x34, 0x56, 0x78 } };
		*pGuid = TestModuleGuid;
		return S_OK;
	}

private:
	ComPtr<IServiceProvider> provider_;
};

int main()
{
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

	{
		ComPtr<IEmsHost> pHost;

		HRESULT hr = CoCreateInstance(
			CLSID_EmsHost,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&pHost)
		);

		if (SUCCEEDED(hr))
		{
			ComPtr<IEmsExtensionModule> pModule = Make<TestModule>();

			hr = pHost->RegisterModule(pModule.Get());

			if (SUCCEEDED(hr))
			{

			}
		}
	}

	CoUninitialize();
	return 0;
}