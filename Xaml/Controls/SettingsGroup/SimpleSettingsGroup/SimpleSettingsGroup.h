#pragma once

#include "Controls/SimpleSettingsGroup.g.h"
#include <wil/result.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>


namespace winrt::SystemExplorer::Xaml::Controls::implementation
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Controls;

    struct SimpleSettingsGroup : SimpleSettingsGroupT<SimpleSettingsGroup>
    {
        SimpleSettingsGroup();

        static constexpr auto PartDescriptionPresenter = L"DescriptionPresenter";

        ContentPresenter _descriptionPresenter;

        class_type::IsEnabledChanged_revoker _enabledChangedRevoker;
        //SimpleSettingsGroup _SimpleSettingsGroup;

        winrt::Microsoft::UI::Xaml::Automation::Peers::AutomationPeer OnCreateAutomationPeer();
        void OnApplyTemplate();
        void SetEnabledState();
        void Update();

        void OnIsEnabledChanged(IInspectable const& sender, DependencyPropertyChangedEventArgs const& e);

        static inline const wil::single_threaded_property<DependencyProperty> HeaderProperty = DependencyProperty::Register(
            L"Header",
            xaml_typename<hstring>(),
            xaml_typename<class_type>(),
            nullptr
        );

        static inline const wil::single_threaded_property<DependencyProperty> DescriptionProperty = DependencyProperty::Register(
            L"Description",
            xaml_typename<IInspectable>(),
            xaml_typename<class_type>(),
            PropertyMetadata(nullptr, [](auto&& d, auto&& e)
            {
                auto self = winrt::get_self<SimpleSettingsGroup>(d.template as<class_type>())->get_strong();
                self->Update();
            })
        );

        [[nodiscard]] hstring Header() const noexcept { return unbox_value<hstring>(GetValue(HeaderProperty)); }
        void Header(hstring const& value) { SetValue(HeaderProperty, box_value(value)); }

        [[nodiscard]] IInspectable Description() const noexcept { return GetValue(DescriptionProperty); }
        void Description(IInspectable const& value) { SetValue(DescriptionProperty, value); }
    };
}

namespace winrt::SystemExplorer::Xaml::Controls::factory_implementation
{
    struct SimpleSettingsGroup : SimpleSettingsGroupT<SimpleSettingsGroup, implementation::SimpleSettingsGroup>
    {
    };
}
