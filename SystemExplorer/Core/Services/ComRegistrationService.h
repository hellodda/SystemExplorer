#pragma once
#include "ISingletonService.h"

namespace winrt::SystemExplorer::Core::Services
{
	struct ComRegistrationService
	{
        static void RegisterDynamicObject(guid const& guid, winrt::com_ptr<IClassFactory> factory);
	};
}

