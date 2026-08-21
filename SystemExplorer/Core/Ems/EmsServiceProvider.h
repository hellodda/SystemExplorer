#pragma once

#include <winrt/base.h>

namespace winrt::SystemExplorer::Core::Ems
{
	struct EmsServiceProvider : winrt::implements<EmsServiceProvider, IServiceProvider>
	{
		HRESULT STDMETHODCALLTYPE QueryService(
			_In_ REFGUID guidService,
			_In_ REFIID riid,
			_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject
		) override;
	};
}