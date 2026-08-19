// dllmain.h: объявление класса модуля.

class CEmsModule : public ATL::CAtlDllModuleT< CEmsModule >
{
public :
	DECLARE_LIBID(LIBID_EmsLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_EMS, "{ea26c6a9-890a-42d0-a169-442d0e83ebd8}")
};

extern class CEmsModule _AtlModule;
