#include "pch.h"
#include "winrt_module_imports.h"
#include "RootViewModel.h"
#if __has_include("ViewModels/RootViewModel.g.cpp")
#include "ViewModels/RootViewModel.g.cpp"
#endif
//#include "../Helpers/Win32/Native/NativeSystem.h"
#include "../Core/Settings/Settings.h"

namespace winrt::SystemExplorer::ViewModels::implementation
{
    RootViewModel::RootViewModel()
    {
        //Core::Settings::UserSettings::AppearanceSettings.SettingChanged([this](std::string_view name, eil::generic_t value)
        //{
        //    if (name == "AppThemeBackgroundImageFit")
        //    {
        //        RaisePropertyChanged(L"AppThemeBackgroundImageFit");
        //    }
        //    else if (name == "AppThemeBackgroundImageVerticalAlignment")
        //    {
        //        RaisePropertyChanged(L"AppThemeBackgroundImageVerticalAlignment");
        //    }
        //    else if (name == "AppThemeBackgroundImageHorizontalAlignment")
        //    {
        //        RaisePropertyChanged(L"AppThemeBackgroundImageHorizontalAlignment");
        //    }
        //    else if (name == "AppThemeBackgroundImageSource")
        //    {
        //        RaisePropertyChanged(L"AppThemeBackgroundImageSource");
        //    }
        //    else if (name == "AppThemeBackgroundImageOpacity")
        //    {
        //        RaisePropertyChanged(L"AppThemeBackgroundImageOpacity");
        //    }
        //});
    }

    winrt::Stretch RootViewModel::AppThemeBackgroundImageFit() const noexcept
    {
        return static_cast<winrt::Stretch>(0);
    }

    winrt::VerticalAlignment RootViewModel::AppThemeBackgroundImageVerticalAlignment() const noexcept
    {
        return static_cast<winrt::VerticalAlignment>(0);
    }

    winrt::HorizontalAlignment RootViewModel::AppThemeBackgroundImageHorizontalAlignment() const noexcept
    {
        return static_cast<winrt::HorizontalAlignment>(0);
    }

    winrt::ImageSource RootViewModel::AppThemeBackgroundImageSource() const noexcept
    {
        auto source = winrt::hstring{ L"" };
        
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
        return 0.0f;
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
