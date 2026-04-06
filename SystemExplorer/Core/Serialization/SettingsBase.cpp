#include "pch.h"
#include "SettingsBase.h"
#if __has_include("Core/Serialization/SettingsBase.g.cpp")
#include "Core/Serialization/SettingsBase.g.cpp"
#endif

namespace winrt::SystemExplorer::Core::Serialization::implementation
{
	void SettingsBase::Initialize(IPropertySet const& values)
	{
		Values = values;

		Values.MapChanged([weak = get_weak()](auto&&, auto&& args) {
            if (auto self = weak.get())
            {
                auto key = args ? args.Key() : hstring{};
                self->RaisePropertyChanged(key);
            }
		});
	}
}
