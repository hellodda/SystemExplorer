#include "pch.h"
#include "winrt_module_imports.h"
#include "RootViewModel.h"
#if __has_include("ViewModels/RootViewModel.g.cpp")
#include "ViewModels/RootViewModel.g.cpp"
#endif
#include "../App.xaml.h"
#include "../Helpers/Win32/Native/NativeSystem.h"
#include "../Core/Settings/UserSettings.h"
#include <winrt/Microsoft.Windows.Storage.Pickers.h>

using namespace winrt::SystemExplorer::Core::Settings;

namespace winrt::SystemExplorer::ViewModels::implementation
{
    RootViewModel::RootViewModel()
    {
        UserSettings::Instance().AppearanceSettings().SettingChanged([this](auto& sender, auto& args)
        {
            onSettingChanged(sender, args);
        });
    }

    Stretch RootViewModel::AppThemeBackgroundImageFit() const noexcept
    {
        return static_cast<Stretch>(UserSettings::Instance().AppearanceSettings().AppThemeBackgroundImageFit());
    }
    VerticalAlignment RootViewModel::AppThemeBackgroundImageVerticalAlignment() const noexcept
    {
        return static_cast<VerticalAlignment>(UserSettings::Instance().AppearanceSettings().AppThemeBackgroundImageVerticalAlignment());
    }
    HorizontalAlignment RootViewModel::AppThemeBackgroundImageHorizontalAlignment() const noexcept
    {
        return static_cast<HorizontalAlignment>(UserSettings::Instance().AppearanceSettings().AppThemeBackgroundImageHorizontalAlignment());
    }
    ImageSource RootViewModel::AppThemeBackgroundImageSource() const noexcept
    {
        auto source = UserSettings::Instance().AppearanceSettings().AppThemeBackgroundImageSource();
        
        if (source.empty())
        {
            return nullptr;
        }

        try
        {
            return BitmapImage{ Uri{ source } };
        }
        catch (...)
        {
            return nullptr;
        }
    }
    float RootViewModel::AppThemeBackgroundImageOpacity() const noexcept
    {
        return UserSettings::Instance().AppearanceSettings().AppThemeBackgroundImageOpacity();
    }

    IAsyncAction RootViewModel::doCreateLiveKernelMemoryDumpAsync()
    {
        using namespace winrt::Microsoft::Windows::Storage::Pickers;
        using namespace winrt::SystemExplorer::Helpers::Win32::Native;

        FileSavePicker picker{ SystemExplorer::CurrentApplication::GetCurrentWindowId() };
        picker.SuggestedFileName(L"KERNEL");
        picker.DefaultFileExtension(L".dmp");

        if (auto savedFile = co_await picker.PickSaveFileAsync())
        {
            try
            {
                co_await NativeSystem::Kernel::CreateLiveKernelMemoryDumpAsync(savedFile.Path().c_str());
            }
            catch (...) {}
        }
    }

    void RootViewModel::onSettingChanged(IInspectable const& sender, Data::EventArguments::SettingChangedEventArgs const& args)
    {
        if (args.SettingName() == L"AppThemeBackgroundImageFit")
        {
            RaisePropertyChanged(L"AppThemeBackgroundImageFit");
        }
        else if (args.SettingName() == L"AppThemeBackgroundImageVerticalAlignment")
        {
            RaisePropertyChanged(L"AppThemeBackgroundImageVerticalAlignment");
        }
        else if (args.SettingName() == L"AppThemeBackgroundImageHorizontalAlignment")
        {
            RaisePropertyChanged(L"AppThemeBackgroundImageHorizontalAlignment");
        }
        else if (args.SettingName() == L"AppThemeBackgroundImageSource")
        {
            RaisePropertyChanged(L"AppThemeBackgroundImageSource");
        }
        else if (args.SettingName() == L"AppThemeBackgroundImageOpacity")
        {
            RaisePropertyChanged(L"AppThemeBackgroundImageOpacity");
        }
    }
}
