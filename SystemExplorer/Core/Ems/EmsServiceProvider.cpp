#include "pch.h"
#include "EmsServiceProvider.h"

#include "EmsProcessService.h"

namespace winrt::SystemExplorer::Core::Ems
{
	HRESULT STDMETHODCALLTYPE EmsServiceProvider::QueryService(
		_In_ REFGUID guidService,
		_In_ REFIID riid,
		_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject
	)
	{
		if (riid == IID_IEmsProcessService)
		{

		}
		return E_NOINTERFACE;
	}
}