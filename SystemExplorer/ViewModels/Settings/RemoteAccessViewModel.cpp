#include "pch.h"
#include "RemoteAccessViewModel.h"
#if __has_include("ViewModels/Settings/RemoteAccessViewModel.g.cpp")
#include "ViewModels/Settings/RemoteAccessViewModel.g.cpp"
#endif

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
    RemoteAccessViewModel::RemoteAccessViewModel()
    {

        winrt::RemoteComputerItem item;
        item.Name(L"TestComputer");
        item.ReadableName(L"AJSIJISJAIJSAI");

        Devices.Append(winrt::box_value(item));
        Devices.Append(winrt::box_value(L"ADD_BUTTON"));

        this->CreateNewDeviceInvoked.invoke();
    }
}
