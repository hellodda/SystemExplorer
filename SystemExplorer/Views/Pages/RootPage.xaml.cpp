#include "pch.h"
#include "winrt_module_imports.h"
#include "RootPage.xaml.h"
#if __has_include("Views/Pages/RootPage.g.cpp")
#include "Views/Pages/RootPage.g.cpp"
#endif
#include "Activities/ProcessesPage.xaml.h"
#include "Settings/SettingsRootPage.xaml.h"
#include <Core/eil/string.h>

namespace winrt::SystemExplorer::Views::Pages::implementation
{
    RootPage::RootPage()
    {
        InitializeComponent();

        
    }

    winrt::IAsyncAction RootPage::PersonPictureTapped(winrt::IInspectable const& sender, winrt::TappedRoutedEventArgs const& args)
    {
        navigateToUri(winrt::Uri{ L"system-explorer://SystemExplorer.Views.Pages.Settings/GeneralPage/UserPage" });

        co_return;
    }

    winrt::IAsyncAction RootPage::NavViewSelectionChanged(winrt::NavigationView const& sender, winrt::NavigationViewSelectionChangedEventArgs const& args)
	{
        if (auto item = args.SelectedItem())
        {
            if (auto pageTag = item.as<winrt::FrameworkElement>().Tag())
            {
				auto pageTagStr = pageTag.try_as<winrt::hstring>();

                if (pageTagStr == L"Settings")
					pageTagStr = xaml_typename<SystemExplorer::Views::Pages::Settings::SettingsRootPage>().Name;
                
                winrt::TypeName typeName{ pageTagStr.value(), winrt::TypeKind::Custom };
                NavFrame().Navigate(typeName);
            }
        }
        co_return;
	}

    [[nosiscard]] winrt::IAsyncAction RootPage::AutoSuggestBox_SuggestionChosen(winrt::IInspectable const& sender, winrt::AutoSuggestBoxSuggestionChosenEventArgs args)
    {
        if (CurrentSuggestionContext != SuggestionContext::Navigation)
            co_return;

        auto item = args.SelectedItem().as<winrt::SystemExplorer::Core::Data::Items::NavigationCatalogItem>();
        navigateToUri(item.Uri());

        co_return;
    }

    [[nodiscard]] winrt::IAsyncAction RootPage::AutoSuggestBox_TextChanged(winrt::IInspectable const& sender, winrt::AutoSuggestBoxTextChangedEventArgs const& args)
    {
        if (CurrentSuggestionContext != SuggestionContext::Navigation)
            co_return;

        if (args.Reason() != winrt::AutoSuggestionBoxTextChangeReason::UserInput)
        {
            co_return;
        }

        auto suggestBox = sender.as<winrt::AutoSuggestBox>();
        auto text = suggestBox.Text();

        if (text.empty())
        {
            suggestBox.ItemsSource(nullptr);
            co_return;
        }

        auto items = ViewModel.TestItems();

        std::wstring_view queryView{ text };

        rapidfuzz::fuzz::CachedPartialRatio scorer(queryView);

        std::vector<winrt::IInspectable> filteredItems;
        filteredItems.reserve(items.Size());

        for (auto const& item : items)
        {
            auto catalogItem = item.as<winrt::SystemExplorer::Core::Data::Items::NavigationCatalogItem>();
            std::wstring_view itemView{ catalogItem.Name() };

            double score = scorer.similarity(itemView);

            if (score > 75.0)
            {
                filteredItems.push_back(item);
            }
        }
        suggestBox.ItemsSource(winrt::single_threaded_vector<winrt::IInspectable>(std::move(filteredItems)));
    }

    void RootPage::navigateToUri(winrt::Uri const& uri)
    {
        auto ns = uri.Host();
        auto path = eil::split_path(uri.Path());
        auto root = path.front();
        auto target = path.back();
        auto rootTypeName = winrt::TypeName{ ns + L"." + root, winrt::TypeKind::Custom };

        NavFrame().Navigate(rootTypeName);

        if (root != target)
        {
            auto targetTypeName = winrt::TypeName{ ns + L"." + target, winrt::TypeKind::Custom };

            if (auto rootPage = NavFrame().Content().try_as<winrt::SystemExplorer::Views::Pages::IUriNavigationSupportPage>())
            {
                rootPage.NavigationFrame().Navigate(targetTypeName);
            }
            if (auto rootPage = NavFrame().Content().try_as<winrt::SystemExplorer::Views::Pages::IBreadcrumbBarNavigationSupportPage>())
            {
                auto vec = winrt::single_threaded_observable_vector<winrt::SystemExplorer::Core::Data::Items::NavigationBreadcrumbBarItem>();
                for (auto i : path)
                {
                    vec.Append(winrt::SystemExplorer::Core::Data::Items::NavigationBreadcrumbBarItem{ winrt::hstring{i}, winrt::TypeName{ winrt::hstring{i}, TypeKind::Custom } });
                }
                rootPage.NavigationBreadcrumbBar().ItemsSource(vec);
            }
        }
    }
}

