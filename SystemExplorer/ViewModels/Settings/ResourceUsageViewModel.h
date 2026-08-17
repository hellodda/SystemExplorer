#pragma once

#include "ViewModels/Settings/ResourceUsageViewModel.g.h"
#include "../ViewModelBase.h"

#include <wil/cppwinrt_authoring.h>
#include <property.h>

namespace winrt
{
    using namespace winrt::Windows::Foundation;
	using namespace winrt::Windows::Foundation::Collections;
}

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
    struct ResourceUsageViewModel : ResourceUsageViewModelT<ResourceUsageViewModel, ViewModelBase>
    {
        ResourceUsageViewModel() = default;

		DECLARE_PROPERTY(uint32_t, InitialMemoryReserve);
		DECLARE_PROPERTY(uint32_t, MemoryLimit);
		DECLARE_PROPERTY(uint32_t, PurgeDelay);
		DECLARE_PROPERTY(bool, UseLargeMemoryPages);

        DECLARE_ONLY_SETTER(int32_t, SelectedMemoryUsageModeIndex);
		wil::single_threaded_property<winrt::IVector<winrt::IInspectable>> MemoryUsageModes = single_threaded_vector<winrt::IInspectable>();
    };
}

namespace winrt::SystemExplorer::ViewModels::Settings::factory_implementation
{
    struct ResourceUsageViewModel : ResourceUsageViewModelT<ResourceUsageViewModel, implementation::ResourceUsageViewModel>
    {
    };
}
