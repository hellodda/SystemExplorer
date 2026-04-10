#pragma once

#include "Views/Pages/Settings/GeneralPage.g.h"

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    struct GeneralPage : GeneralPageT<GeneralPage>
    {
        GeneralPage()
        {
        
        }
    };
}

FACTORY(winrt::SystemExplorer::Views::Pages::Settings, GeneralPage);