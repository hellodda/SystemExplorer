#pragma once

#include "Controls/DataTable.g.h"
#include <unordered_set>
#include <wil/cppwinrt_authoring.h>

namespace winrt::SystemExplorer::Xaml::Controls::implementation
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Controls;

    struct DataTable : DataTableT<DataTable>
    {
        DataTable() = default;

        // TODO: We should cache this result and update if column properties change
        bool IsAnyColumnAuto();

        // TODO: Check with Sergio if there's a better structure here, as I don't need a Dictionary like ConditionalWeakTable
        std::unordered_set<winrt::SystemExplorer::Xaml::Controls::DataRow>& Rows();

        void ColumnResized();

        //// TODO: Would we want this named 'Spacing' instead if we support an Orientation in the future for columns being items instead of rows?
        bool ColumnSpacing() const;
        void ColumnSpacing(double value);

        static const wil::single_threaded_property<DependencyProperty> ColumnSpacingProperty;

        Size MeasureOverride(Size availableSize);

        Size ArrangeOverride(Size finalSize);

    private:
        std::unordered_set<winrt::SystemExplorer::Xaml::Controls::DataRow> _rows;
    };
}

namespace winrt::SystemExplorer::Xaml::Controls::factory_implementation
{
    struct DataTable : DataTableT<DataTable, implementation::DataTable>
    {
    };
}
