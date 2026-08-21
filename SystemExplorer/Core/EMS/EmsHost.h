#pragma once
#include <winrt/base.h>
#include <mutex> 

#include <Core/Eil/event.h>
#include <absl/container/inlined_vector.h>

namespace winrt::SystemExplorer::Core::Ems
{
    struct EmsModuleRegistry : winrt::implements<EmsModuleRegistry, IEmsHost>
    {
        HRESULT STDMETHODCALLTYPE RegisterModule(
            _In_ IEmsExtensionModule* pModule
        ) override;

    private:

        HRESULT ValidateModule(
            _In_ IEmsExtensionModule* pModule
        );

    private:
        absl::InlinedVector<IEmsExtensionModule*, 3> registeredModules_{ };
    };

    struct EmsModuleRegistryClassFactory : winrt::implements<EmsModuleRegistryClassFactory, IClassFactory>
    {
        HRESULT STDMETHODCALLTYPE CreateInstance(
            _In_opt_  IUnknown* pUnkOuter,
            _In_  REFIID riid,
            _COM_Outptr_  void** ppvObject
        ) override;

        HRESULT STDMETHODCALLTYPE LockServer(
            BOOL fLock
        ) override;
    };
}