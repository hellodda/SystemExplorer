#pragma once

#include "Views/Pages/Activities/ProcessesPage.g.h"

namespace winrt::SystemExplorer::Views::Pages::Activities::implementation
{
    typedef struct I_NATIVE_DATA_STRUCT
    {
        int id;
        const wchar_t* name;
    } *PI_NATIVE_DATA_STRUCT;


    struct ProcessesPage : ProcessesPageT<ProcessesPage>
    {
        ProcessesPage()
        {
            InitializeComponent();

		
        }
		wil::single_threaded_property<ViewModels::Activities::ProcessesViewModel> ViewModel;
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Activities, ProcessesPage);
