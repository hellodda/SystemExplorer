#include "pch.h"
#include "EnumHelper.h"
#include <winrt/SystemExplorer.Helpers.h>
#include <stdexcept>

namespace winrt::SystemExplorer::Helpers
{
    winrt::hstring EnumHelper::Map(winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType value)
    {
        switch (value)
        {
            case winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType::Static: return winrt::SystemExplorer::Helpers::StringsHelper::Static();
            case winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType::Mica: return winrt::SystemExplorer::Helpers::StringsHelper::Mica();
            case winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType::MicaAlt: return winrt::SystemExplorer::Helpers::StringsHelper::MicaAlt();
            case winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType::Acrylic: return winrt::SystemExplorer::Helpers::StringsHelper::Acrylic();
            case winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType::ThinAcrylic: return winrt::SystemExplorer::Helpers::StringsHelper::ThinAcrylic();
            default: throw winrt::hresult_invalid_argument(L"Invalid value for enum BackdropMaterialType");
        }
    }

    template <>
    winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType EnumHelper::Map<winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType>(winrt::hstring const& value)
    {
        if (value == winrt::SystemExplorer::Helpers::StringsHelper::Static()) return winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType::Static;
        if (value == winrt::SystemExplorer::Helpers::StringsHelper::Mica()) return winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType::Mica;
        if (value == winrt::SystemExplorer::Helpers::StringsHelper::MicaAlt()) return winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType::MicaAlt;
        if (value == winrt::SystemExplorer::Helpers::StringsHelper::Acrylic()) return winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType::Acrylic;
        if (value == winrt::SystemExplorer::Helpers::StringsHelper::ThinAcrylic()) return winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType::ThinAcrylic;
        throw winrt::hresult_invalid_argument(L"Invalid string mapping for enum BackdropMaterialType");
    }

}
