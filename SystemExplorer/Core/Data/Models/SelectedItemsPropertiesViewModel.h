#pragma once

#include "Core/Data/Models/SelectedItemsPropertiesViewModel.g.h"

#include <factory.h>

namespace winrt::SystemExplorer::Core::Data::Models::implementation
{
    struct SelectedItemsPropertiesViewModel : SelectedItemsPropertiesViewModelT<SelectedItemsPropertiesViewModel>
    {
        SelectedItemsPropertiesViewModel() = default;

        wil::single_threaded_rw_property<hstring> ItemName;
        wil::single_threaded_rw_property<hstring> ItemLocation;
        wil::single_threaded_rw_property<hstring> ItemFileNameWin32;
        wil::single_threaded_rw_property<hstring> ItemFileName;
        wil::single_threaded_rw_property<hstring> ItemLifeTime;
        wil::single_threaded_rw_property<hstring> ItemCommandLine;
        wil::single_threaded_rw_property<hstring> ItemDirectory;
        wil::single_threaded_rw_property<hstring> ItemParentConsole;
        wil::single_threaded_rw_property<hstring> ItemParentProcess;
        wil::single_threaded_rw_property<hstring> ItemProtection;
        wil::single_threaded_rw_property<hstring> ItemVersion;
        wil::single_threaded_rw_property<hstring> ItemType;

    };
}
FACTORY(winrt::SystemExplorer::Core::Data::Models, SelectedItemsPropertiesViewModel);