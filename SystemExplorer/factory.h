#pragma once

#define FACTORY(NAMESPACE, CLASS) \
namespace NAMESPACE::factory_implementation \
{ \
	struct CLASS : CLASS ## T<CLASS, implementation::CLASS> \
	{ \
	}; \
}

#define COM_FACTORY(NAMESPACE, CLASS) \
namespace NAMESPACE::factory_implementation \
{ \
	struct CLASS ## Factory : winrt::implements<CLASS ## Factory, IClassFactory> \
	{ \
        HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown* outer, REFIID riid, void** ppv) noexcept override \
        { \
            if (outer) return CLASS_E_NOAGGREGATION; \
            try \
            { \
                return winrt::make_self<implementation::CLASS>().as(riid, ppv); \
            } \
            catch (...) \
            { \
                return to_hresult(); \
            } \
        } \
        HRESULT STDMETHODCALLTYPE LockServer(BOOL) noexcept override { return S_OK; } \
    }; \
} 