#pragma once
#include "Ems_i.h"
#include "EmsApi.h"
#include "UndefInterfaces.h"
#include "EmsProcessService.h"


#define BEGIN_PROXY_SERVICE_MAP() \
    STDMETHOD(QueryService)(_In_ REFGUID guidService, _In_ REFIID riid, _Outptr_ void** ppvObject) override { \
        if (!ppvObject) return E_POINTER; \
        *ppvObject = nullptr; \
        HRESULT _hr = E_NOINTERFACE;

#define PROXY_SERVICE_ENTRY(ServiceGuid, ServiceEnum, ProxyClass) \
        if (guidService == ServiceGuid) { \
            if (!IsServiceSupported(ServiceEnum)) return E_NOINTERFACE; \
            CComObject<ProxyClass>* _service = nullptr; \
            _hr = CComObject<ProxyClass>::CreateInstance(&_service); \
            if (FAILED(_hr)) return _hr; \
            _hr = _service->QueryInterface(riid, ppvObject); \
            return _hr; \
        }

#define END_PROXY_SERVICE_MAP() \
        return E_NOTIMPL; \
    }

using namespace ATL;

class ATL_NO_VTABLE CEmsServiceProvider :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CEmsServiceProvider, &CLSID_EmsServiceProvider>,
	public IServiceProvider
{
public:

	CEmsServiceProvider() = default;

	DECLARE_REGISTRY_RESOURCEID(IDR_EMSSERVICEPROVIDER)

	CUSTOM_BEGIN_COM_MAP(CEmsServiceProvider)
		COM_INTERFACE_ENTRY(IServiceProvider)
	END_COM_MAP()

	BEGIN_PROXY_SERVICE_MAP()
		PROXY_SERVICE_ENTRY(IID_IEmsProcessService, EmsServiceProcess, CEmsProcessService)
	END_PROXY_SERVICE_MAP()

	HRESULT STDMETHODCALLTYPE Host(
		_In_ HANDLE hHost
	);

private:

	BOOL IsServiceSupported(EMS_SERVICE_TYPE service);

private:
	HANDLE host_{ nullptr };
};
