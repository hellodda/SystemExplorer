#pragma once
#include <winrt/base.h>

#include <winrt/SystemExplorer.Core.Data.Enums.h>

namespace winrt::SystemExplorer::Helpers
{
    struct EnumHelper
    {
        [[nodiscard]] static winrt::hstring Map(winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType value);

        template <typename EnumT>
        [[nodiscard]] static EnumT Map(winrt::hstring const& value);

    private:
        EnumHelper() = default;
    };

    template <>
    winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType EnumHelper::Map<winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType>(winrt::hstring const& value);
}
