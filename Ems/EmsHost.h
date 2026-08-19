#pragma once
#include "Ems_i.h"
#include "UndefInterfaces.h"

using namespace ATL;

class CEmsHost :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CEmsHost, &CLSID_EmsHost>,
	public IEmsHost
{
public:

	CEmsHost() = default;

	DECLARE_REGISTRY_RESOURCEID(IDR_EMSHOST)

	BEGIN_COM_MAP(CEmsHost)
		COM_INTERFACE_ENTRY(IEmsHost)
	END_COM_MAP()

public:

	STDMETHOD(RegisterModule)(
		_In_ IEmsExtensionModule* pModule
	);

private:

	HRESULT STDMETHODCALLTYPE TryConnect(
		_In_ IEmsExtensionModule* pModule
	);
	
	HRESULT STDMETHODCALLTYPE ValidateModule(
		_In_ IEmsExtensionModule* pModule
	);

private:
	HANDLE hostHandle_{ nullptr };
};