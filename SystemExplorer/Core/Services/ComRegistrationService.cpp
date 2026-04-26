#include "pch.h"
#include "ComRegistrationService.h"

namespace winrt::SystemExplorer::Core::Services
{
	void ComRegistrationService::RegisterDynamicObject(guid guid, winrt::com_ptr<IClassFactory> factory)
	{
        DWORD cookie;
        check_hresult(CoRegisterClassObject(
            guid,
            factory.get(),
            CLSCTX_LOCAL_SERVER,
            REGCLS_MULTIPLEUSE,
            &cookie
        ));
	}
}
