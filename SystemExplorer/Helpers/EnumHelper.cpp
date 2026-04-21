#include "pch.h"
#include "EnumHelper.h"
#include <stdexcept>
#include <winrt/SystemExplorer.Helpers.h>

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
        }
        throw winrt::hresult_invalid_argument(L"Invalid value for enum BackdropMaterialType");
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

    winrt::hstring EnumHelper::Map(winrt::SystemExplorer::Core::Data::Enums::HorizontalAlignment value)
    {
        switch (value)
        {
            case winrt::SystemExplorer::Core::Data::Enums::HorizontalAlignment::Left: return winrt::SystemExplorer::Helpers::StringsHelper::Left();
            case winrt::SystemExplorer::Core::Data::Enums::HorizontalAlignment::Center: return winrt::SystemExplorer::Helpers::StringsHelper::Center();
            case winrt::SystemExplorer::Core::Data::Enums::HorizontalAlignment::Right: return winrt::SystemExplorer::Helpers::StringsHelper::Right();
        }
        throw winrt::hresult_invalid_argument(L"Invalid value for enum HorizontalAlignment");
    }

    template <>
    winrt::SystemExplorer::Core::Data::Enums::HorizontalAlignment EnumHelper::Map<winrt::SystemExplorer::Core::Data::Enums::HorizontalAlignment>(winrt::hstring const& value)
    {
        if (value == winrt::SystemExplorer::Helpers::StringsHelper::Left()) return winrt::SystemExplorer::Core::Data::Enums::HorizontalAlignment::Left;
        if (value == winrt::SystemExplorer::Helpers::StringsHelper::Center()) return winrt::SystemExplorer::Core::Data::Enums::HorizontalAlignment::Center;
        if (value == winrt::SystemExplorer::Helpers::StringsHelper::Right()) return winrt::SystemExplorer::Core::Data::Enums::HorizontalAlignment::Right;
        throw winrt::hresult_invalid_argument(L"Invalid string mapping for enum HorizontalAlignment");
    }

    winrt::hstring EnumHelper::Map(winrt::SystemExplorer::Core::Data::Enums::InformationUpdateSpeed value)
    {
        switch (value)
        {
            case winrt::SystemExplorer::Core::Data::Enums::InformationUpdateSpeed::High: return winrt::SystemExplorer::Helpers::StringsHelper::High();
            case winrt::SystemExplorer::Core::Data::Enums::InformationUpdateSpeed::Default: return winrt::SystemExplorer::Helpers::StringsHelper::Default();
            case winrt::SystemExplorer::Core::Data::Enums::InformationUpdateSpeed::Low: return winrt::SystemExplorer::Helpers::StringsHelper::Low();
            case winrt::SystemExplorer::Core::Data::Enums::InformationUpdateSpeed::Suspended: return winrt::SystemExplorer::Helpers::StringsHelper::Suspended();
        }
        throw winrt::hresult_invalid_argument(L"Invalid value for enum InformationUpdateSpeed");
    }

    template <>
    winrt::SystemExplorer::Core::Data::Enums::InformationUpdateSpeed EnumHelper::Map<winrt::SystemExplorer::Core::Data::Enums::InformationUpdateSpeed>(winrt::hstring const& value)
    {
        if (value == winrt::SystemExplorer::Helpers::StringsHelper::High()) return winrt::SystemExplorer::Core::Data::Enums::InformationUpdateSpeed::High;
        if (value == winrt::SystemExplorer::Helpers::StringsHelper::Default()) return winrt::SystemExplorer::Core::Data::Enums::InformationUpdateSpeed::Default;
        if (value == winrt::SystemExplorer::Helpers::StringsHelper::Low()) return winrt::SystemExplorer::Core::Data::Enums::InformationUpdateSpeed::Low;
        if (value == winrt::SystemExplorer::Helpers::StringsHelper::Suspended()) return winrt::SystemExplorer::Core::Data::Enums::InformationUpdateSpeed::Suspended;
        throw winrt::hresult_invalid_argument(L"Invalid string mapping for enum InformationUpdateSpeed");
    }

    winrt::hstring EnumHelper::Map(winrt::SystemExplorer::Core::Data::Enums::Stretch value)
    {
        switch (value)
        {
            case winrt::SystemExplorer::Core::Data::Enums::Stretch::None: return winrt::SystemExplorer::Helpers::StringsHelper::None();
            case winrt::SystemExplorer::Core::Data::Enums::Stretch::Fill: return winrt::SystemExplorer::Helpers::StringsHelper::Fill();
            case winrt::SystemExplorer::Core::Data::Enums::Stretch::Uniform: return winrt::SystemExplorer::Helpers::StringsHelper::Uniform();
            case winrt::SystemExplorer::Core::Data::Enums::Stretch::UniformToFill: return winrt::SystemExplorer::Helpers::StringsHelper::UniformToFill();
        }
        throw winrt::hresult_invalid_argument(L"Invalid value for enum Stretch");
    }

    template <>
    winrt::SystemExplorer::Core::Data::Enums::Stretch EnumHelper::Map<winrt::SystemExplorer::Core::Data::Enums::Stretch>(winrt::hstring const& value)
    {
        if (value == winrt::SystemExplorer::Helpers::StringsHelper::None()) return winrt::SystemExplorer::Core::Data::Enums::Stretch::None;
        if (value == winrt::SystemExplorer::Helpers::StringsHelper::Fill()) return winrt::SystemExplorer::Core::Data::Enums::Stretch::Fill;
        if (value == winrt::SystemExplorer::Helpers::StringsHelper::Uniform()) return winrt::SystemExplorer::Core::Data::Enums::Stretch::Uniform;
        if (value == winrt::SystemExplorer::Helpers::StringsHelper::UniformToFill()) return winrt::SystemExplorer::Core::Data::Enums::Stretch::UniformToFill;
        throw winrt::hresult_invalid_argument(L"Invalid string mapping for enum Stretch");
    }

    winrt::hstring EnumHelper::Map(winrt::SystemExplorer::Core::Data::Enums::VerticalAlignment value)
    {
        switch (value)
        {
            case winrt::SystemExplorer::Core::Data::Enums::VerticalAlignment::Top: return winrt::SystemExplorer::Helpers::StringsHelper::Top();
            case winrt::SystemExplorer::Core::Data::Enums::VerticalAlignment::Center: return winrt::SystemExplorer::Helpers::StringsHelper::Center();
            case winrt::SystemExplorer::Core::Data::Enums::VerticalAlignment::Bottom: return winrt::SystemExplorer::Helpers::StringsHelper::Bottom();
        }
        throw winrt::hresult_invalid_argument(L"Invalid value for enum VerticalAlignment");
    }

    template <>
    winrt::SystemExplorer::Core::Data::Enums::VerticalAlignment EnumHelper::Map<winrt::SystemExplorer::Core::Data::Enums::VerticalAlignment>(winrt::hstring const& value)
    {
        if (value == winrt::SystemExplorer::Helpers::StringsHelper::Top()) return winrt::SystemExplorer::Core::Data::Enums::VerticalAlignment::Top;
        if (value == winrt::SystemExplorer::Helpers::StringsHelper::Center()) return winrt::SystemExplorer::Core::Data::Enums::VerticalAlignment::Center;
        if (value == winrt::SystemExplorer::Helpers::StringsHelper::Bottom()) return winrt::SystemExplorer::Core::Data::Enums::VerticalAlignment::Bottom;
        throw winrt::hresult_invalid_argument(L"Invalid string mapping for enum VerticalAlignment");
    }

}
