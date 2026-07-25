#pragma once

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Globalization.h> 
#endif

#include <Core/Data/Items/AppLanguageItem.h> 

namespace winrt::SystemExplorer::Core::Services
{
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Windows::Globalization;

    using namespace winrt::SystemExplorer::Core::Data::Items;

	struct AppLanguageService final
	{
        [[nodiscard]] static IVector<AppLanguageItem> SupportedLanguages() noexcept;

        [[nodiscard]] static AppLanguageItem PreferredLanguage() noexcept;

        [[nodiscard]] static bool IsPreferredLanguageRtl();

        [[nodiscard]] static bool TryChange(int32_t index);

        [[nodiscard]] static bool TryChange(winrt::hstring const& code);
	};
}

