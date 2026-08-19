#pragma once

#include "framework.h"
#include "shobjidl.h"

using namespace ATL;

#ifdef  DEBUGLOG

#define CUSTOM_BEGIN_COM_MAP(x) public: \
	typedef x _ComMapClass; \
	static HRESULT WINAPI _Cache(_In_ void* pv, _In_ REFIID iid, _COM_Outptr_result_maybenull_ void** ppvObject, _In_ DWORD_PTR dw) throw()\
	{\
		_ComMapClass* p = (_ComMapClass*)pv;\
		p->Lock();\
		HRESULT hRes = E_FAIL; \
		__try \
		{ \
			hRes = ATL::CComObjectRootBase::_Cache(pv, iid, ppvObject, dw);\
		} \
		__finally \
		{ \
			p->Unlock();\
		} \
		return hRes;\
	}\
	IUnknown* _GetRawUnknown() throw() \
	{ ATLASSERT(_GetEntries()[0].pFunc == _ATL_SIMPLEMAPENTRY); return (IUnknown*)((INT_PTR)this+_GetEntries()->dw); } \
	_ATL_DECLARE_GET_UNKNOWN(x)\
	HRESULT _InternalQueryInterface( \
		_In_ REFIID iid, \
		_COM_Outptr_ void** ppvObject) throw() \
	{ \
		HRESULT res = this->InternalQueryInterface(this, _GetEntries(), iid, ppvObject); \
		OLECHAR* guidString; \
		(void)StringFromCLSID(iid, &guidString); \
		std::wcout << L"QueryInterface: " << guidString << L" = " << res << std::endl; \
		::CoTaskMemFree(guidString); \
		return res; \
	} \
	const static ATL::_ATL_INTMAP_ENTRY* WINAPI _GetEntries() throw() { \
	static const ATL::_ATL_INTMAP_ENTRY _entries[] = { DEBUG_QI_ENTRY(x)

#else

#define CUSTOM_BEGIN_COM_MAP(x) BEGIN_COM_MAP(x)

#endif //  DEBUGLOG