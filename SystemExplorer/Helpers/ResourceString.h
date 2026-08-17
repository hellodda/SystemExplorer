#pragma once

#include "Helpers/ResourceString.g.h"
#include <wil/cppwinrt_helpers.h>

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Media;
    using namespace winrt::Microsoft::Windows::ApplicationModel::Resources;
}

namespace winrt::SystemExplorer::Helpers::implementation
{
    struct ResourceString : ResourceStringT<ResourceString>
    {
        ResourceString() = default;

        wil::single_threaded_rw_property<winrt::hstring> Key;

        [[nodiscard]] winrt::IInspectable ProvideValue();

        [[nodiscard]] winrt::IInspectable ProvideValue([[maybe_unused]] winrt::IXamlServiceProvider const& provider);

        [[nodiscard]] static winrt::hstring LoadResource(winrt::hstring const& key);

    private:
        static inline winrt::ResourceLoader loader_;
    };
}

namespace winrt::SystemExplorer::Helpers::factory_implementation
{
    struct ResourceString : ResourceStringT<ResourceString, implementation::ResourceString>
    {
    };
}
