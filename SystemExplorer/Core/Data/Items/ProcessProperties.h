#pragma once

#include "Core/Data/Items/ProcessProperties.g.h"
#include "ProcessItem.h"
#include <Core/Data/Models/SelectedItemsPropertiesViewModel.h>
#include <factory.h>

namespace winrt::SystemExplorer::Core::Data::Items::implementation
{
    using namespace winrt::SystemExplorer::Core::Data::Models;

    struct ProcessProperties : ProcessPropertiesT<ProcessProperties>
    {
        ProcessProperties(SelectedItemsPropertiesViewModel const& viewModel, Items::ProcessItem const& item)
        {
            viewModel.ItemName(item.Name());
            viewModel.ItemParentProcess(to_hstring(item.ParentId()));
        }

        ProcessProperties() = default;
    };
}
FACTORY(winrt::SystemExplorer::Core::Data::Items, ProcessProperties);
