#include "pch.h"
#include "RootViewModel.h"
#if __has_include("ViewModels/RootViewModel.g.cpp")
#include "ViewModels/RootViewModel.g.cpp"
#endif
#include "../App.xaml.h"
#include "../Helpers/Win32/Native/NativeSystem.h"

#include <winrt/Microsoft.Windows.Storage.Pickers.h>

namespace winrt::SystemExplorer::ViewModels::implementation
{
    Stretch RootViewModel::AppThemeBackgroundImageFit() const noexcept
    {
        return static_cast<Stretch>(settings_.AppThemeBackgroundImageFit());
    }
    VerticalAlignment RootViewModel::AppThemeBackgroundImageVerticalAlignment() const noexcept
    {
        return static_cast<VerticalAlignment>(settings_.AppThemeBackgroundImageVerticalAlignment());
    }
    HorizontalAlignment RootViewModel::AppThemeBackgroundImageHorizontalAlignment() const noexcept
    {
        return static_cast<HorizontalAlignment>(settings_.AppThemeBackgroundImageHorizontalAlignment());
    }
    ImageSource RootViewModel::AppThemeBackgroundImageSource() const noexcept
    {
        auto source = settings_.AppThemeBackgroundImageSource();
        
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
        return settings_.AppThemeBackgroundImageOpacity();
    }

    IAsyncAction RootViewModel::doCreateLiveKernelMemoryDumpAsync()
    {
        using namespace winrt::Microsoft::Windows::Storage::Pickers;
        using namespace winrt::SystemExplorer::Helpers::Win32::Native;

        FileSavePicker picker{ SystemExplorer::CurrentApplication::GetWindowId() };
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

    void RootViewModel::onSettingChanged(IInspectable const& sender, Core::Data::EventArguments::SettingChangedEventArgs const& args)
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
