#include "pch.h"
#include "EmsHost.h"
#include "EmsApi.h"
#include "EmsServiceProvider.h"

OBJECT_ENTRY_AUTO(__uuidof(EmsHost), CEmsHost)

STDMETHODIMP CEmsHost::RegisterModule(
	_In_ IEmsExtensionModule* pModule
)
{
	auto hr = ValidateModule(pModule);

	if (SUCCEEDED(hr))
	{
		return TryConnect(pModule);
	}
	return hr;
}

STDMETHODIMP CEmsHost::ValidateModule(
	_In_ IEmsExtensionModule* pModule
)
{
	if (!pModule)
		return E_POINTER;

	GUID guid = { 0 };
	HRESULT hr = pModule->ModuleGuid(&guid);
	if (FAILED(hr))
		return hr;

	if (guid == GUID_NULL)
		return E_INVALIDARG; 

	CComBSTR bstrName;
	hr = pModule->ModuleName(&bstrName);
	if (FAILED(hr))
		return hr;

	if (!bstrName || SysStringLen(bstrName) == 0)
		return E_INVALIDARG;

	return S_OK;
}

STDMETHODIMP CEmsHost::TryConnect(
	_In_ IEmsExtensionModule* pModule
)
{
	if (!pModule)
		return E_POINTER;

	EMS_MODULE_CONNECT connect = { 0 };

	HRESULT hr = pModule->ModuleGuid(&connect.Guid);
	if (FAILED(hr))
		return hr;

	CComBSTR bstrName;
	hr = pModule->ModuleName(&bstrName);
	if (FAILED(hr))
		return hr;

	if (bstrName.m_str)
	{
		StringCchCopyW(connect.Name, ARRAYSIZE(connect.Name), bstrName);
	}

	CComBSTR bstrAuthor;
	hr = pModule->ModuleAuthor(&bstrAuthor);
	if (FAILED(hr))
		return hr;

	if (bstrAuthor.m_str)
	{
		StringCchCopyW(connect.Author, ARRAYSIZE(connect.Author), bstrAuthor);
	}

	CComBSTR bstrDesc;
	hr = pModule->ModuleDescription(&bstrDesc);
	if (FAILED(hr))
		return hr;

	if (bstrDesc.m_str)
	{
		StringCchCopyW(connect.Description, ARRAYSIZE(connect.Description), bstrDesc);
	}

	auto status = EmsConnectToHost(&hostHandle_, &connect);

	if (NT_ERROR(status))
		return HRESULT_FROM_NT(status);

	CComObject<CEmsServiceProvider>* pProvider;

	CComObject<CEmsServiceProvider>::CreateInstance(&pProvider);
	pProvider->Host(hostHandle_);

	pModule->SetSite(pProvider);

	return S_OK;
}