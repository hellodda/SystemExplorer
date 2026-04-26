#pragma once
#include <winrt/Windows.AI.Actions.h>
#include <winrt/Windows.AI.Actions.Provider.h>


/// <summary>
/// poka v planax...
/// </summary>

constexpr winrt::guid ProcessesActionProviderGuid = { 0x9f46d58d, 0xdffa, 0x4c00, { 0x85, 0xc2, 0xbf, 0xfb, 0x13, 0xbd, 0xcb, 0x6a } };

namespace winrt::SystemExplorer::Core::AI::Actions::implementation
{
    using namespace winrt::Windows::Foundation;

    using namespace winrt::Windows::AI::Actions;
    using namespace winrt::Windows::AI::Actions::Provider;

    struct __declspec(uuid("9f46d58d-dffa-4c00-85c2-bffb13bdcb6a")) ProcessesActionProvider : implements<ProcessesActionProvider, IActionProvider>
    {
        ProcessesActionProvider() = default;

        IAsyncAction InvokeAsync(ActionInvocationContext const& context);

    private:
        static IAsyncAction endProcessAction(ActionInvocationContext const& context);
        //static IAsyncAction createProcessAction();
        //static IAsyncAction getDetailInformationOfProcessAction();
    };
}

namespace winrt::SystemExplorer::Core::AI::Actions::factory_implementation
{
    struct ProcessesActionProviderFactory : implements<ProcessesActionProviderFactory, IClassFactory>
    {
        HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown* outer, REFIID riid, void** ppv) noexcept override
        {
            if (outer) return CLASS_E_NOAGGREGATION;
            try
            {
                return winrt::make_self<implementation::ProcessesActionProvider>().as(riid, ppv);
            }
            catch (...)
            {
                return to_hresult();
            }
        }
        HRESULT STDMETHODCALLTYPE LockServer(BOOL) noexcept override { return S_OK; }
    };
}

