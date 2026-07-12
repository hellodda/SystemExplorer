#include "pch.h"
#include "SharedSettings.h"

namespace winrt::SystemExplorer::Core::Settings
{
    using result = std::vector<AppearanceProfile>;

    concurrency::task<result> SharedSettings::GetAppearanceProfilesAsync()
    {
        return concurrency::create_task([]() -> result
        {
            result profiles;

            LOG_IF_FAILED(wil::ResultFromException([&]()
            {
                auto settingsFolder = winrt::Windows::Storage::ApplicationData::Current().GetPublisherCacheFolder(SharedFolderName);
                fs::path rootPath(settingsFolder.Path().c_str());

                if (!fs::exists(rootPath)) return;

                for (const auto& entry : fs::directory_iterator(rootPath))
                {
                    if (entry.is_regular_file() && entry.path().extension() == L".dat")
                    {
                        LOG_IF_FAILED(wil::ResultFromException([&]()
                        {
                            std::ifstream inFile(entry.path(), std::ios::in | std::ios::binary);

                            THROW_HR_IF(E_FAIL, !inFile.is_open());

                            AppearanceProfile profile;
                            profile.AppName = ReadStringBinary(inFile);
                            profile.AppBackgroundColor = ReadStringBinary(inFile);
                            inFile.read(reinterpret_cast<char*>(&profile.AppTheme), sizeof(profile.AppTheme));
                            inFile.read(reinterpret_cast<char*>(&profile.AppBackdropType), sizeof(profile.AppBackdropType));

                            THROW_HR_IF(E_FAIL, inFile.fail());

                            profiles.push_back(std::move(profile));
                        }));
                    }
                }
            }));

            return profiles;
        });
    }

    concurrency::task<void> SharedSettings::SaveAppearanceProfileAsync(AppearanceProfile const& profile)
    {
        return concurrency::create_task([profile]()
        {
            LOG_IF_FAILED(wil::ResultFromException([&]()
            {
                auto settingsFolder = winrt::Windows::Storage::ApplicationData::Current().GetPublisherCacheFolder(SharedFolderName);
                fs::path rootPath(settingsFolder.Path().c_str());

                fs::path filePath = rootPath / (profile.AppName + L".dat");

                std::ofstream outFile(filePath, std::ios::out | std::ios::trunc | std::ios::binary);

                THROW_HR_IF_MSG(E_FAIL, !outFile.is_open(), "Cannot open shared settings");

                WriteStringBinary(outFile, profile.AppName);
                WriteStringBinary(outFile, profile.AppBackgroundColor);
                outFile.write(reinterpret_cast<const char*>(&profile.AppTheme), sizeof(profile.AppTheme));
                outFile.write(reinterpret_cast<const char*>(&profile.AppBackdropType), sizeof(profile.AppBackdropType));

                THROW_HR_IF_MSG(E_FAIL, outFile.fail(), "Failed to write data to .dat");

                outFile.close();
            }));
        });
    }
}