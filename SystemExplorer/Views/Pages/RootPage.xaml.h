#pragma once
#include "Views/Pages/RootPage.g.h"

#include <ViewModels/RootViewModel.h>
#include "Settings/SettingsRootPage.xaml.h"
#include <winrt/SystemExplorer.Views.Pages.h>

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Controls;
    using namespace winrt::Windows::UI::Xaml::Interop;
}

namespace winrt::SystemExplorer::Views::Pages::implementation
{
    struct RootPage : RootPageT<RootPage>
    {
        RootPage();

        enum class SuggestionContext
        {
            Navigation,
            Search
        };

        wil::single_threaded_property<ViewModels::RootViewModel> ViewModel;
        wil::single_threaded_property<SuggestionContext> CurrentSuggestionContext = SuggestionContext::Navigation;

        [[nodiscard]] winrt::IAsyncAction PersonPictureTapped(winrt::IInspectable const& sender, winrt::TappedRoutedEventArgs const& args);

        [[nodiscard]] winrt::IAsyncAction NavViewSelectionChanged(winrt::NavigationView const& sender, winrt::NavigationViewSelectionChangedEventArgs const& args);

        [[nosiscard]] winrt::IAsyncAction AutoSuggestBox_SuggestionChosen(winrt::IInspectable const& sender, winrt::AutoSuggestBoxSuggestionChosenEventArgs args);

        [[nodiscard]] winrt::IAsyncAction AutoSuggestBox_TextChanged(winrt::IInspectable const& sender, winrt::AutoSuggestBoxTextChangedEventArgs const& args);
    private:
        void navigateToUri(winrt::Uri const& uri);
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages, RootPage);
