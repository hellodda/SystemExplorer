#include "pch.h"
#include "AppLanguageService.h"

#include <Helpers/Common.h>

namespace winrt::SystemExplorer::Core::Services
{
    static IVector<AppLanguageItem> supportedLanguages_{ nullptr };
    static AppLanguageItem preferredLanguage_{ L"", false };

    AppLanguageService::AppLanguageService()
    {
        auto appLanguages = ViewToVector(ApplicationLanguages::ManifestLanguages());

        std::vector<AppLanguageItem> languageItems;
        languageItems.reserve(appLanguages.Size());

        for (auto const& code : appLanguages)
        {
            languageItems.emplace_back(AppLanguageItem{ code, false });
        }

        std::sort(languageItems.begin(), languageItems.end(),
            [](AppLanguageItem const& a, AppLanguageItem const& b) -> bool
            {
                bool aIsDefault = a.Code().empty();
                bool bIsDefault = b.Code().empty();

                if (aIsDefault != bIsDefault)
                    return aIsDefault;

                return a.Name() < b.Name();
            });

        auto current = AppLanguageItem(ApplicationLanguages::PrimaryLanguageOverride(), false);

        size_t currentIndex{ 0 };
        for (size_t i = 0; i < languageItems.size(); ++i)
        {
            if (languageItems[i].Name() == current.Name())
            {
                currentIndex = i;
                break;
            }
        }

        hstring installedCode = L"en-US";
        auto manifestView = ApplicationLanguages::ManifestLanguages();
        if (manifestView.Size() > 0)
            installedCode = manifestView.GetAt(0);

        auto systemLanguage = AppLanguageItem(installedCode, true);

        bool hasSystemItem = false;
        for (auto const& lang : languageItems)
        {
            if (std::wstring(lang.Name().c_str()).find(systemLanguage.Name()) != std::wstring::npos)
            {
                hasSystemItem = true;
                break;
            }
        }

        if (hasSystemItem)
            languageItems[0] = std::move(systemLanguage);
        else
            languageItems[0] = AppLanguageItem(L"en-US", true);

        supportedLanguages_ = winrt::single_threaded_vector(std::move(languageItems));
        preferredLanguage_ = supportedLanguages_.GetAt(static_cast<uint32_t>(currentIndex));
    }

    IVector<AppLanguageItem> AppLanguageService::SupportedLanguages() noexcept
    {
        return supportedLanguages_;
    }

    AppLanguageItem AppLanguageService::PreferredLanguage() noexcept
    {
        return preferredLanguage_;
    }

    bool AppLanguageService::IsPreferredLanguageRtl()
    {
        auto const& code = preferredLanguage_.Code();
        if (code.empty())
            return false;

        DWORD readingLayout{ 0 };

        if (GetLocaleInfoEx(
            code.c_str(),
            LOCALE_IREADINGLAYOUT | LOCALE_RETURN_NUMBER,
            reinterpret_cast<LPWSTR>(&readingLayout),
            sizeof(readingLayout) / sizeof(WCHAR)) > 0)
        {
            return readingLayout == 1;
        }

        return false;
    }

    bool AppLanguageService::TryChange(int32_t index)
    {
        using namespace winrt::Windows::Globalization;

        if (index < 0 || static_cast<uint32_t>(index) >= supportedLanguages_.Size() ||
            preferredLanguage_ == supportedLanguages_.GetAt(static_cast<uint32_t>(index)))
        {
            return false;
        }

        preferredLanguage_ = supportedLanguages_.GetAt(static_cast<uint32_t>(index));

        ApplicationLanguages::PrimaryLanguageOverride(
            index == 0 ? L"" : preferredLanguage_.Code()
        );

        return true;
    }

    bool AppLanguageService::TryChange(hstring const& code)
    {
        using namespace winrt::Windows::Globalization;

        if (code.empty())
            return false;

        auto lang = AppLanguageItem(code, false);

        uint32_t foundIndex = 0;
        bool found = false;

        for (uint32_t i = 0; i < supportedLanguages_.Size(); ++i)
        {
            if (supportedLanguages_.GetAt(i).Name() == lang.Name())
            {
                foundIndex = i;
                found = true;
                break;
            }
        }

        if (!found)
            return false;

        if (preferredLanguage_ == supportedLanguages_.GetAt(foundIndex))
            return false;

        preferredLanguage_ = supportedLanguages_.GetAt(foundIndex);

        ApplicationLanguages::PrimaryLanguageOverride(
            foundIndex == 0 ? L"" : preferredLanguage_.Code()
        );

        return true;
    }
}