#pragma once

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Globalization.h> 
#endif

#include <Core/Data/Items/AppLanguageItem.h> 

namespace winrt
{
    using namespace winrt::SystemExplorer::Core::Data::Items;
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Windows::Globalization;
}

namespace winrt::SystemExplorer::Core::Services
{
	struct AppLanguageService final
	{
        AppLanguageService();

        [[nodiscard]] static winrt::IVector<winrt::AppLanguageItem> SupportedLanguages() noexcept;

        [[nodiscard]] static winrt::AppLanguageItem PreferredLanguage() noexcept;

        [[nodiscard]] static bool IsPreferredLanguageRtl();

        [[nodiscard]] static bool TryChange(int32_t index);

        [[nodiscard]] static bool TryChange(winrt::hstring const& code);
	};
}

