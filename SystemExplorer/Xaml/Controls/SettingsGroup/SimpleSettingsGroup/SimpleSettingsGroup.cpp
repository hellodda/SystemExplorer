#include "pch.h"
#include "winrt_module_imports.h"
#include "SimpleSettingsGroup.h"
#if __has_include("Xaml/SimpleSettingsGroup.g.cpp")
#include "Xaml/SimpleSettingsGroup.g.cpp"
#endif
#include "SimpleSettingsGroupAutomationPeer.h"

namespace winrt::SystemExplorer::Xaml::implementation
{
	bool IsNullOrEmptyString(IInspectable const& object)
	{
		if (object == nullptr)
			return true;

		if (auto objString = object.try_as<winrt::hstring>(); objString && objString->empty())
			return true;
		
		return false;
	}

	SimpleSettingsGroup::SimpleSettingsGroup()
	{
		DefaultStyleKey(box_value(xaml_typename<class_type>()));
	}

	winrt::Microsoft::UI::Xaml::Automation::Peers::AutomationPeer SimpleSettingsGroup::OnCreateAutomationPeer()
	{
		return make<winrt::SystemExplorer::Xaml::implementation::SimpleSettingsGroupAutomationPeer>(*this);
	}

	void SimpleSettingsGroup::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();

		SetEnabledState();
		Update();

		_enabledChangedRevoker = IsEnabledChanged(winrt::auto_revoke, { this, &SimpleSettingsGroup::OnIsEnabledChanged });
		_descriptionPresenter = this->GetTemplateChild(PartDescriptionPresenter).try_as<ContentPresenter>();

	}
	void SimpleSettingsGroup::SetEnabledState()
	{
		VisualStateManager::GoToState(*this, IsEnabled() ? L"Normal" : L"Disabled", true);
	}
	void SimpleSettingsGroup::Update()
	{
		_descriptionPresenter.Visibility(IsNullOrEmptyString(Description()) ? Visibility::Collapsed : Visibility::Visible);
	}
	void SimpleSettingsGroup::OnIsEnabledChanged([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
	{
		SetEnabledState();
	}
}
