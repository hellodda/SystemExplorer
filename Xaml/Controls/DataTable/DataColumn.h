#pragma once

#include "Controls/DataColumn.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <winrt/SystemExplorer.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/XamlToolkit.WinUI.h>
#include <winrt/XamlToolkit.WinUI.Controls.h>

namespace winrt::SystemExplorer::Xaml::Controls::implementation
{
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Controls;
    using namespace winrt::Microsoft::UI::Xaml::Input;

    struct DataColumn : DataColumnT<DataColumn>
    {
        static constexpr auto PartColumnSizer = L"PART_ColumnSizer";

        DataColumn();

        bool CanResize() const;
        void CanResize(bool value);

        static const wil::single_threaded_property<DependencyProperty> CanResizeProperty;

        GridLength DesiredWidth() const;
        void DesiredWidth(GridLength value);

        static const wil::single_threaded_property<DependencyProperty> DesiredWidthProperty;

        wil::single_threaded_rw_property<double> MaxChildDesiredWidth;

        GridLength CurrentWidth() const;

        void OnApplyTemplate();

        void ColumnSizer_ManipulationDelta(winrt::Windows::Foundation::IInspectable const& sender, ManipulationDeltaRoutedEventArgs const& e);

        void ColumnSizer_ManipulationCompleted(winrt::Windows::Foundation::IInspectable const& sender, ManipulationCompletedRoutedEventArgs const& e);

        void ColumnResizedByUserSizer();

    private:
        static void DesiredWidth_PropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        static inline GridLength StarLength = GridLength(1, GridUnitType::Star);

        GridLength _currentWidth;

        winrt::XamlToolkit::WinUI::Controls::ContentSizer _columnSizer{ nullptr };

        winrt::weak_ref<winrt::SystemExplorer::Xaml::Controls::DataTable> _parent;

        UIElement::ManipulationDelta_revoker _columnSizerManipulationDeltaRevoker;
        UIElement::ManipulationCompleted_revoker _columnSizerManipulationCompletedRevoker;
    };
}

namespace winrt::SystemExplorer::Xaml::Controls::factory_implementation
{
    struct DataColumn : DataColumnT<DataColumn, implementation::DataColumn>
    {
    };
}
