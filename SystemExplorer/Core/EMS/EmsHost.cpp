#include "pch.h"
#include "EmsHost.h"

namespace winrt::SystemExplorer::Core::Ems
{
	HRESULT EmsModuleRegistry::RegisterModule(
		_In_ IEmsExtensionModule* pModule
	)
	{
		if (ValidateModule(pModule) > 0)
		{
			pModule->SetSite(nullptr);
		}
		return E_INVALIDARG;
	}

	HRESULT EmsModuleRegistry::ValidateModule(
		_In_ IEmsExtensionModule* pModule
	)
	{
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE EmsModuleRegistryClassFactory::CreateInstance(
		_In_opt_  IUnknown* pUnkOuter,
		_In_  REFIID riid,
		_COM_Outptr_  void** ppvObject
	)
	{
		if (pUnkOuter)
		{
			*ppvObject = nullptr;
			return CLASS_E_NOAGGREGATION;
		}
		static winrt::com_ptr<EmsModuleRegistry> instance = winrt::make_self<EmsModuleRegistry>();

		return instance.as(riid, ppvObject);
	}

	HRESULT STDMETHODCALLTYPE EmsModuleRegistryClassFactory::LockServer(
		BOOL fLock
	)
	{
		if (fLock)
		{
			++winrt::get_module_lock();
		}
		else
		{
			--winrt::get_module_lock();
		}
		return S_OK;
	}
}