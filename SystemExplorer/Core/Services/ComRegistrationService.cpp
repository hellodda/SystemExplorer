#include "pch.h"
#include "ComRegistrationService.h"

namespace winrt::SystemExplorer::Core::Services
{
	void ComRegistrationService::RegisterDynamicObject(guid const& guid, winrt::com_ptr<IClassFactory> factory)
	{
        DWORD cookie;
        THROW_IF_FAILED_MSG(CoRegisterClassObject(
            guid,
            factory.get(),
            CLSCTX_LOCAL_SERVER,
            REGCLS_MULTIPLEUSE,
            &cookie
        ), "Dynamic object registration failed.");
	}
}
