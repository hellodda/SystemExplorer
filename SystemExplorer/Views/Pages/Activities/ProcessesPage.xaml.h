#pragma once

#include "Views/Pages/Activities/ProcessesPage.g.h"
#include <factory.h>
#include <Core/Plugins/PluginServer.h>

namespace winrt::SystemExplorer::Views::Pages::Activities::implementation
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml::Controls;

    struct ProcessesPage : ProcessesPageT<ProcessesPage>
    {
        ProcessesPage()
        {
            InitializeComponent();

            g_server.Map(ALPC_CMD_CREATE_BUTTON, {
                .MinVersion = ALPC_API_VERSION_IGNORE,
                .Callback = [this](alpc::AlpcRequestMessage const& request, alpc::AlpcResponseMessage& response)
                {
                     auto information = request.ReadAs<BUTTON_INFORMATION>().value();

                    if (information.Location == BUTTON_LOCATION::blProcessesListMenuFlyout)
                    {

                        dispatcher.TryEnqueue([this, info = information]()
                        {
                            MenuFlyoutItem item;
                            item.Text(to_hstring(info.Text));

                            ProcessMenuFlyoutObject().Items().Append(item);
                        });
                    }
                }
            });
        }
        IAsyncAction ProcessMenuFlyoutOpened(IInspectable const& sender, IInspectable const&);

		wil::single_threaded_property<ViewModels::Activities::ProcessesViewModel> ViewModel;

        Microsoft::UI::Dispatching::DispatcherQueue dispatcher = Microsoft::UI::Dispatching::DispatcherQueue::GetForCurrentThread();
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Activities, ProcessesPage);
