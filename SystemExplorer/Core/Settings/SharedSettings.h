#pragma once
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.h>
#include <ppl.h>
#include <pplawait.h>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

namespace winrt::SystemExplorer::Core::Settings
{
    namespace fs = std::filesystem;

    struct AppearanceProfile
    {
        std::wstring AppName;
        std::wstring AppBackgroundColor;
        int32_t AppTheme;
        int32_t AppBackdropType;
    };

    struct SharedSettings
    {
    private:
        static constexpr const wchar_t* SharedFolderName = L"ProjectK";

        static void WriteStringBinary(std::ofstream& os, const std::wstring& str)
        {
            size_t len = str.size();
            os.write(reinterpret_cast<const char*>(&len), sizeof(len));
            if (len > 0)
            {
                os.write(reinterpret_cast<const char*>(str.data()), len * sizeof(wchar_t));
            }
        }

        static std::wstring ReadStringBinary(std::ifstream& is)
        {
            size_t len = 0;
            is.read(reinterpret_cast<char*>(&len), sizeof(len));

            if (len > 0 && len < 4096)
            {
                std::wstring str(len, L'\0');
                is.read(reinterpret_cast<char*>(str.data()), len * sizeof(wchar_t));
                return str;
            }
            return L"";
        }

    public:
        static concurrency::task<result> GetAppearanceProfilesAsync();
        static concurrency::task<void> SaveAppearanceProfileAsync(AppearanceProfile const& profile);
    };
}