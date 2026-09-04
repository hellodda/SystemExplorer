#include "pch.h"
#include "SettingsGroup.h"
#if __has_include("Xaml/SettingsGroup.g.cpp")
#include "Xaml/SettingsGroup.g.cpp"
#endif

namespace winrt::SystemExplorer::Xaml::implementation
{
	SettingsGroup::SettingsGroup()
	{
		DefaultStyleKey();

		Items(winrt::single_threaded_observable_vector<winrt::IInspectable>());
		Items().VectorChanged(winrt::auto_revoke, { this, &SettingsGroup::Items_CollectionChanged });
	}

	void SettingsGroup::OnApplyTemplate()
	{
		itemsRepeater_ = GetTemplateChild(PartItemsRepeater).try_as<winrt::ItemsRepeater>();
		contentPresenter_ = GetTemplateChild(PartContentPresenter).try_as<winrt::ContentPresenter>();
		headerPresenter_ = GetTemplateChild(PartHeaderPresenter).try_as<winrt::ContentPresenter>();
		descriptionPresenter_ = GetTemplateChild(PartDescriptionPresenter).try_as<winrt::ContentPresenter>();
		rootGrid_ = GetTemplateChild(PartRootGrid).try_as<winrt::Grid>();

		if (itemsRepeater_)
		{
			RaiseItemsChanged();
		}

		SetEnabledState();

		isEnabledChangedRevoker_ = IsEnabledChanged(winrt::auto_revoke, { this, &SettingsGroup::SettingsGroup_IsEnabledChanged });

		RaiseHeaderIconChanged();
		RaiseHeaderChanged();
		RaiseDescriptionChanged();
		RaiseContentChanged();

		RootGridCornerRadius(GerRootGridCornerRadius());

		base_type::OnApplyTemplate();
	}

	void SettingsGroup::SetEnabledState()
	{
		winrt::VisualStateManager::GoToState(*this, IsEnabled() ? L"Normal" : L"Disabled", true);
	}
}
