#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("Views/Windows/MainWindow.g.cpp")
#include "Views/Windows/MainWindow.g.cpp"
#endif

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;


namespace winrt::SystemExplorer::Views::Windows::implementation
{
	void MainWindow::OnSystemBackdropSettingChanged(IInspectable const& sender, Core::Data::EventArguments::SettingChangedEventArgs const& args)
	{
        if (args.SettingName() == L"BackdropMaterial")
        {
            auto type = unbox_value<Core::Data::Enums::BackdropMaterialType>(args.NewValue());

            switch (type)
            {
            case Core::Data::Enums::BackdropMaterialType::Static:
                this->SystemBackdrop(nullptr);
                break;
            case Core::Data::Enums::BackdropMaterialType::Mica:
                this->SystemBackdrop(Microsoft::UI::Xaml::Media::MicaBackdrop{});
                break;
            case Core::Data::Enums::BackdropMaterialType::MicaAlt:
            {
                auto backdrop = Microsoft::UI::Xaml::Media::MicaBackdrop{};
                backdrop.Kind(Microsoft::UI::Composition::SystemBackdrops::MicaKind::BaseAlt);

                this->SystemBackdrop(backdrop);
                break;
            }
            case Core::Data::Enums::BackdropMaterialType::Acrylic:
                this->SystemBackdrop(Microsoft::UI::Xaml::Media::DesktopAcrylicBackdrop{});
                break;
            case Core::Data::Enums::BackdropMaterialType::ThinAcrylic:
                this->SystemBackdrop(Microsoft::UI::Xaml::Media::DesktopAcrylicBackdrop{});
                break;
            default:
                break;
            }
        }
    }
}

