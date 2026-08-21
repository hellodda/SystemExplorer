#include "pch.h"
#include "winrt_module_imports.h"
#include "RootViewModel.h"
#if __has_include("ViewModels/RootViewModel.g.cpp")
#include "ViewModels/RootViewModel.g.cpp"
#endif

#include "../Core/Settings/Settings.h"

namespace winrt::SystemExplorer::ViewModels::implementation
{
    RootViewModel::RootViewModel()
    {
        Core::Settings::UserSettings::AppearanceSettings.SettingChanged->operator()([this](std::wstring_view name, winrt::IInspectable const& value)
        {
            if (name == L"AppThemeBackgroundImageFit")
            {
                RaisePropertyChanged(L"AppThemeBackgroundImageFit");
            }
            else if (name == L"AppThemeBackgroundImageVerticalAlignment")
            {
                RaisePropertyChanged(L"AppThemeBackgroundImageVerticalAlignment");
            }
            else if (name == L"AppThemeBackgroundImageHorizontalAlignment")
            {
                RaisePropertyChanged(L"AppThemeBackgroundImageHorizontalAlignment");
            }
            else if (name == L"AppThemeBackgroundImageSource")
            {
                RaisePropertyChanged(L"AppThemeBackgroundImageSource");
            }
            else if (name == L"AppThemeBackgroundImageOpacity")
            {
                RaisePropertyChanged(L"AppThemeBackgroundImageOpacity");
            }
        });
    }

    winrt::Stretch RootViewModel::AppThemeBackgroundImageFit() const noexcept
    {
        return static_cast<winrt::Stretch>(Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageFit());
    }

    winrt::VerticalAlignment RootViewModel::AppThemeBackgroundImageVerticalAlignment() const noexcept
    {
        return static_cast<winrt::VerticalAlignment>(Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageVerticalAlignment());
    }

    winrt::HorizontalAlignment RootViewModel::AppThemeBackgroundImageHorizontalAlignment() const noexcept
    {
        return static_cast<winrt::HorizontalAlignment>(Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageHorizontalAlignment());
    }

    winrt::ImageSource RootViewModel::AppThemeBackgroundImageSource() const noexcept
    {
        auto source = Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageSource();
        
        if (source.empty())
        {
            return nullptr;
        }

        try
        {
            return winrt::BitmapImage{ Uri{ source } };
        }
        catch (...)
        {
            return nullptr;
        }
    }

    float RootViewModel::AppThemeBackgroundImageOpacity() const noexcept
    {
        return Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageOpacity();
    }

    winrt::IAsyncAction RootViewModel::doCreateLiveKernelMemoryDumpAsync()
    {
      //  using namespace winrt::Microsoft::Windows::Storage::Pickers;
      ///*  using namespace winrt::SystemExplorer::Helpers::Win32::Native;*/

      //  FileSavePicker picker{ SystemExplorer::CurrentApplication::GetCurrentWindowId() };
      //  picker.SuggestedFileName(L"KERNEL");
      //  picker.DefaultFileExtension(L".dmp");

      //  if (auto savedFile = co_await picker.PickSaveFileAsync())
      //  {
      //      try
      //      {
      //         /* co_await NativeSystem::Kernel::CreateLiveKernelMemoryDumpAsync(savedFile.Path().c_str());*/
      //          co_return;
      //      }
      //      catch (...) {}
      //  }
        co_return;
    }
}
