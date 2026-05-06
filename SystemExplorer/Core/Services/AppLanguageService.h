#pragma once
#include "ISingletonService.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Globalization.h> 
#include <Core/Data/Items/AppLanguageItem.h> 

namespace winrt::SystemExplorer::Core::Services
{
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Windows::Globalization;

    using namespace winrt::SystemExplorer::Core::Data::Items;

	struct AppLanguageService : ISingletonService<AppLanguageService>
	{
        AppLanguageService();

        IVector<AppLanguageItem> SupportedLanguages() noexcept;

        AppLanguageItem PreferredLanguage() noexcept;

        bool IsPreferredLanguageRtl();

        bool TryChange(int32_t index);

        bool TryChange(winrt::hstring const& code);

	};
}

