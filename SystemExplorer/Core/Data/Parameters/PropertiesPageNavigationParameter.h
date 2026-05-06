#pragma once

#include "Core/Data/Parameters/PropertiesPageNavigationParameter.g.h"

#include <factory.h>

namespace winrt::SystemExplorer::Core::Data::Parameters::implementation
{
    using namespace winrt::Windows::Foundation;

    struct PropertiesPageNavigationParameter : PropertiesPageNavigationParameterT<PropertiesPageNavigationParameter>
    {
        PropertiesPageNavigationParameter() = default;
        PropertiesPageNavigationParameter(IInspectable parameter)
            : Parameter(parameter) {}

        wil::single_threaded_rw_property<IInspectable> Parameter;
    };
}
FACTORY(winrt::SystemExplorer::Core::Data::Parameters, PropertiesPageNavigationParameter);