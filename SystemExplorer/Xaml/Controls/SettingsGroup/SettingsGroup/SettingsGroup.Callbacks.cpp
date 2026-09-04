#include "pch.h"
#include "SettingsGroup.h"

namespace winrt::SystemExplorer::Xaml::implementation
{
	void SettingsGroup::OnItemsChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		if (auto settingsGroup = d.try_as<SystemExplorer::Xaml::implementation::SettingsGroup>())
		{
			settingsGroup->RaiseItemsChanged();
		}
	}

	void SettingsGroup::OnContentChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		if (auto settingsGroup = d.try_as<SystemExplorer::Xaml::implementation::SettingsGroup>())
		{
			settingsGroup->RaiseContentChanged();
		}
	}

	void SettingsGroup::OnHeaderIconChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		if (auto settingsGroup = d.try_as<SystemExplorer::Xaml::implementation::SettingsGroup>())
		{
			settingsGroup->RaiseHeaderIconChanged();
		}
	}

	void SettingsGroup::OnHeaderChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		if (auto settingsGroup = d.try_as<SystemExplorer::Xaml::implementation::SettingsGroup>())
		{
			settingsGroup->RaiseHeaderChanged();
		}
	}

	void SettingsGroup::OnDescriptionChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		if (auto settingsGroup = d.try_as<SystemExplorer::Xaml::implementation::SettingsGroup>())
		{
			settingsGroup->RaiseDescriptionChanged();
		}
	}

	void SettingsGroup::RaiseItemsChanged()
	{
		if (itemsRepeater_)
		{
			RootGridCornerRadius(GerRootGridCornerRadius());

			if (rootGrid_)
			{
				rootGrid_.CornerRadius(RootGridCornerRadius());

				if (Items().Size() > 0)
				{
					rootGrid_.BorderThickness(winrt::Thickness{ 1, 1, 1, 0 });
				}
				else
				{
					rootGrid_.BorderThickness(winrt::Thickness{ 1, 1, 1, 1 });
				}
			}

			auto size = Items().Size();
			for (int32_t i = 0; i < size; ++i)
			{
				auto item = Items().GetAt(i);

				if (auto control = item.try_as<winrt::Control>())
				{
					winrt::CornerRadius cornerRadius{ 0 };
					winrt::Thickness borderThickness{ 1, 1, 1, 0 };

					if (i == size - 1)
					{
						cornerRadius = winrt::CornerRadius{ 0, 0, 8, 8 };
						borderThickness = winrt::Thickness{ 1, 1, 1, 1 };
					}
					control.BorderThickness(borderThickness);
					control.CornerRadius(cornerRadius);
					control.Padding(winrt::Thickness{ 25, 0, 25, 0 });
					control.Margin(winrt::Thickness{ 0 });
				}
			}
			itemsRepeater_.ItemsSource(Items());
		}
	}

	void SettingsGroup::RaiseContentChanged()
	{
		if (contentPresenter_)
		{
			contentPresenter_.Visibility(Content() ? winrt::Visibility::Visible : winrt::Visibility::Collapsed);
		}
	}

	void SettingsGroup::RaiseHeaderIconChanged()
	{
		if (auto headerIconPresenter = GetTemplateChild(PartHeaderIconPresenter).try_as<winrt::FrameworkElement>())
		{
			headerIconPresenter.Visibility(HeaderIcon() ? winrt::Visibility::Visible : winrt::Visibility::Collapsed);
		}
	}

	void SettingsGroup::RaiseHeaderChanged()
	{
		if (headerPresenter_)
		{
			headerPresenter_.Visibility(Header() ? winrt::Visibility::Visible : winrt::Visibility::Collapsed);
		}
	}

	void SettingsGroup::RaiseDescriptionChanged()
	{
		if (descriptionPresenter_)
		{
			descriptionPresenter_.Visibility(Description() ? winrt::Visibility::Visible : winrt::Visibility::Collapsed);
		}
	}

	void SettingsGroup::Items_CollectionChanged(winrt::IInspectable const& s, winrt::IVectorChangedEventArgs const& e)
	{
		RaiseItemsChanged();
	}

	void SettingsGroup::SettingsGroup_IsEnabledChanged(winrt::IInspectable const& s, DependencyPropertyChangedEventArgs const& e)
	{
		winrt::VisualStateManager::GoToState(*this, IsEnabled() ? L"Normal" : L"Disabled", true);
	}
}