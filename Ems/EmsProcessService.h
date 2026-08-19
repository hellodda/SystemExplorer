#pragma once
#include "Ems_i.h"
#include "UndefInterfaces.h"

using namespace ATL;

class ATL_NO_VTABLE CEmsProcessService :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CEmsProcessService, &CLSID_EmsProcessService>,
	public IEmsProcessService,
	public IEmsService
{
public:

	CEmsProcessService() = default;

	CUSTOM_BEGIN_COM_MAP(CEmsProcessService)
		COM_INTERFACE_ENTRY(IEmsProcessService)
		COM_INTERFACE_ENTRY(IEmsService)
	END_COM_MAP()

public:

	STDMETHOD(TerminateProcess)(
		_In_ DWORD dwPid
	) override;

private:
	HANDLE host_{ nullptr };
};