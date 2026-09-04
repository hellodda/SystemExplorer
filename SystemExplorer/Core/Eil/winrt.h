#pragma once
#include "string.h"
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <roapi.h>

#define raise_property_changed() RaisePropertyChanged(eil::get_function_namew(__FUNCTIONW__))

namespace eil
{
	[[nodiscard]] bool is_valid_type(winrt::Windows::UI::Xaml::Interop::TypeName const& type)
	{
        winrt::com_ptr<winrt::Windows::Foundation::IInspectable> factory;
        HRESULT hr = ::RoGetActivationFactory(
            static_cast<HSTRING>(winrt::get_abi(type.Name)),
            winrt::guid_of<winrt::Windows::Foundation::IInspectable>(),
            winrt::put_abi(factory)
        );

        if (SUCCEEDED(hr) && factory != nullptr)
        {
            return true; 
        }

        try
        {
            auto app = winrt::Microsoft::UI::Xaml::Application::Current();
            if (app)
            {
                if (auto provider = app.try_as<winrt::Microsoft::UI::Xaml::Markup::IXamlMetadataProvider>())
                {
                    if (provider.GetXamlType(type) != nullptr)
                    {
                        return true;
                    }
                }
            }
        }
        catch (...) { }

        return false;
	}
}
