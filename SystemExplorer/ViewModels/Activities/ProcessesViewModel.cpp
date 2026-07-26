#include "pch.h"
#include "winrt_module_imports.h"
#include "ProcessesViewModel.h"

#if __has_include("ViewModels/Activities/ProcessesViewModel.g.cpp")
#include "ViewModels/Activities/ProcessesViewModel.g.cpp"
#endif
#include <App.xaml.h>

namespace winrt::SystemExplorer::ViewModels::Activities::implementation
{
    ProcessesViewModel::ProcessesViewModel()
    {
    
    }

    void ProcessesViewModel::SelectedProcess(ProcessItem const& value) noexcept
    {
        if (value != SelectedProcess_)
        {
            SelectedProcess_ = value;
            TerminateProcessCommand.NotifyCanExecuteChanged();
            EfficiencyModeCommand.NotifyCanExecuteChanged();

            //RaisePropertyChanged(L"SelectedProcess");
        }
    }

    // commands impl
    winrt::IAsyncAction ProcessesViewModel::doTerminateProcessAsync()
    {
      /*  if (!SelectedProcess_) co_return;
        const auto pid = SelectedProcess_.Pid();

        try
        {
            manager_->Terminate(pid);
        }
        catch (const wil::ResultException&) {}*/
        co_return;
    }

    winrt::IAsyncAction ProcessesViewModel::doSetEfficiencyModeAsync()
    {
       /* if (!SelectedProcess_) co_return;
        const auto pid = SelectedProcess_.Pid();

        try
        {
            if (!NativeProcess::IsEfficiencyModeEnabled(pid))
            {
                manager_->EnableEfficiencyMode(pid);
            }
            else
            {
                manager_->DisableEfficiencyMode(pid);
            }
        }
        catch (const wil::ResultException&) {}*/
        co_return;
    }

    winrt::IAsyncAction ProcessesViewModel::doRestartProcessAsync()
    {
    /*    if (!SelectedProcess_) co_return;
        manager_->Restart(SelectedProcess_.Pid());*/
        co_return;
    }

    winrt::IAsyncAction ProcessesViewModel::doOpenProcessDetailsWindowAsync()
    {
     /*   if (SelectedProcess_)
        {
            ProcessPropertiesHelper::OpenPropertiesWindow(SelectedProcess_);
        }*/
        co_return;
    }

    winrt::IAsyncAction ProcessesViewModel::doOpenProcessLocationAsync()
    {
     /*   using namespace winrt::Windows::Storage;
        if (!SelectedProcess_) co_return;

        try
        {
            const auto path = NativeProcess::GetProcessImageName(SelectedProcess_.Pid());
            auto targetFile = co_await StorageFile::GetFileFromPathAsync(path);

            if (auto parentFolder = co_await targetFile.GetParentAsync())
            {
                winrt::Windows::System::FolderLauncherOptions options;
                options.ItemsToSelect().Append(targetFile);

                co_await winrt::Windows::System::Launcher::LaunchFolderAsync(parentFolder, options);
            }
        }
        catch (const hresult_error&) {}
        catch (const wil::ResultException&) {}*/
        co_return;
    }

    /*IAsyncAction ProcessesViewModel::doDumpProcessMemoryAsync(MINIDUMP_TYPE dumpType)
    {
        if (!SelectedProcess_) co_return;

        const auto pid = SelectedProcess_.Pid();
        const auto processName = SelectedProcess_.Name();

        auto processItem = provider_->GetProcess(pid);
        if (!processItem) co_return;

        FileSavePicker picker{ SystemExplorer::CurrentApplication::GetCurrentWindowId() };

        picker.SuggestedFileName(processName + L"_memorydump");
        picker.DefaultFileExtension(L".dmp");

        if (auto savedFile = co_await picker.PickSaveFileAsync())
        {
            try
            {
                SeCreateDumpFileProcess(savedFile.Path().c_str(), processItem.get(), dumpType);
            }
            catch (...) {}
        }
    }*/

    winrt::IAsyncAction ProcessesViewModel::showErrorDialogAsync(const hstring& message)
    {
        MessageBox(NULL, message.c_str(), L"Action failed", MB_OK | MB_ICONERROR);
        co_return;
    }
}