#pragma once
#include <winrt/base.h>
#include <winrt/SystemExplorer.Core.Data.Enums.h>
#include <type_traits>
#include <string_view>

namespace winrt::SystemExplorer::Helpers
{
    struct EnumHelper
    {
        EnumHelper() = delete;
        [[nodiscard]] static winrt::hstring Map(winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType value);
        [[nodiscard]] static winrt::hstring Map(winrt::SystemExplorer::Core::Data::Enums::HorizontalAlignment value);
        [[nodiscard]] static winrt::hstring Map(winrt::SystemExplorer::Core::Data::Enums::InformationUpdateSpeed value);
        [[nodiscard]] static winrt::hstring Map(winrt::SystemExplorer::Core::Data::Enums::Stretch value);
        [[nodiscard]] static winrt::hstring Map(winrt::SystemExplorer::Core::Data::Enums::VerticalAlignment value);

        template <typename EnumT>
            requires std::is_enum_v<EnumT>
        [[nodiscard]] static EnumT Map(winrt::hstring const& value);
    };
    template <>
    winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType EnumHelper::Map<winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType>(winrt::hstring const& value);
    template <>
    winrt::SystemExplorer::Core::Data::Enums::HorizontalAlignment EnumHelper::Map<winrt::SystemExplorer::Core::Data::Enums::HorizontalAlignment>(winrt::hstring const& value);
    template <>
    winrt::SystemExplorer::Core::Data::Enums::InformationUpdateSpeed EnumHelper::Map<winrt::SystemExplorer::Core::Data::Enums::InformationUpdateSpeed>(winrt::hstring const& value);
    template <>
    winrt::SystemExplorer::Core::Data::Enums::Stretch EnumHelper::Map<winrt::SystemExplorer::Core::Data::Enums::Stretch>(winrt::hstring const& value);
    template <>
    winrt::SystemExplorer::Core::Data::Enums::VerticalAlignment EnumHelper::Map<winrt::SystemExplorer::Core::Data::Enums::VerticalAlignment>(winrt::hstring const& value);
}
