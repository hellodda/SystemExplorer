

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 07:14:07 2038
 */
/* Compiler settings for Ems.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0628 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __Ems_i_h__
#define __Ems_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if defined(_CONTROL_FLOW_GUARD_XFG)
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

/* Forward Declarations */ 

#ifndef __IEmsExtensionModule_FWD_DEFINED__
#define __IEmsExtensionModule_FWD_DEFINED__
typedef interface IEmsExtensionModule IEmsExtensionModule;

#endif 	/* __IEmsExtensionModule_FWD_DEFINED__ */


#ifndef __IEmsHost_FWD_DEFINED__
#define __IEmsHost_FWD_DEFINED__
typedef interface IEmsHost IEmsHost;

#endif 	/* __IEmsHost_FWD_DEFINED__ */


#ifndef __IEmsMessagingTransport_FWD_DEFINED__
#define __IEmsMessagingTransport_FWD_DEFINED__
typedef interface IEmsMessagingTransport IEmsMessagingTransport;

#endif 	/* __IEmsMessagingTransport_FWD_DEFINED__ */


#ifndef __IEmsService_FWD_DEFINED__
#define __IEmsService_FWD_DEFINED__
typedef interface IEmsService IEmsService;

#endif 	/* __IEmsService_FWD_DEFINED__ */


#ifndef __IEmsProcessService_FWD_DEFINED__
#define __IEmsProcessService_FWD_DEFINED__
typedef interface IEmsProcessService IEmsProcessService;

#endif 	/* __IEmsProcessService_FWD_DEFINED__ */


#ifndef __EmsServiceProvider_FWD_DEFINED__
#define __EmsServiceProvider_FWD_DEFINED__

#ifdef __cplusplus
typedef class EmsServiceProvider EmsServiceProvider;
#else
typedef struct EmsServiceProvider EmsServiceProvider;
#endif /* __cplusplus */

#endif 	/* __EmsServiceProvider_FWD_DEFINED__ */


#ifndef __EmsProcessService_FWD_DEFINED__
#define __EmsProcessService_FWD_DEFINED__

#ifdef __cplusplus
typedef class EmsProcessService EmsProcessService;
#else
typedef struct EmsProcessService EmsProcessService;
#endif /* __cplusplus */

#endif 	/* __EmsProcessService_FWD_DEFINED__ */


#ifndef __EmsHost_FWD_DEFINED__
#define __EmsHost_FWD_DEFINED__

#ifdef __cplusplus
typedef class EmsHost EmsHost;
#else
typedef struct EmsHost EmsHost;
#endif /* __cplusplus */

#endif 	/* __EmsHost_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __EmsLib_LIBRARY_DEFINED__
#define __EmsLib_LIBRARY_DEFINED__

/* library EmsLib */
/* [version][uuid] */ 

typedef 
enum tag_EMS_MODULE_ACCESS_LEVEL
    {
        EmsModuleLowAccessLevel	= 0,
        EmsModuleNormalAccessLevel	= ( EmsModuleLowAccessLevel + 1 ) ,
        EmsModuleHighAccessLevel	= ( EmsModuleNormalAccessLevel + 1 ) 
    } 	EMS_MODULE_ACCESS_LEVEL;


EXTERN_C const IID LIBID_EmsLib;

#ifndef __IEmsExtensionModule_INTERFACE_DEFINED__
#define __IEmsExtensionModule_INTERFACE_DEFINED__

/* interface IEmsExtensionModule */
/* [object][uuid] */ 


EXTERN_C const IID IID_IEmsExtensionModule;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("543A8588-E987-49DC-AE36-C23AB338DC69")
    IEmsExtensionModule : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetSite( 
            /* [in] */ IServiceProvider *pSP) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryClose( 
            /* [retval][out] */ BOOL *pfCanClose) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ModuleName( 
            /* [retval][out] */ BSTR *pBstrName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ModuleAuthor( 
            /* [retval][out] */ BSTR *pBstrAuthor) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ModuleDescription( 
            /* [retval][out] */ BSTR *pBstrDescription) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ModuleGuid( 
            /* [retval][out] */ GUID *pGuid) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IEmsExtensionModuleVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEmsExtensionModule * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEmsExtensionModule * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEmsExtensionModule * This);
        
        DECLSPEC_XFGVIRT(IEmsExtensionModule, SetSite)
        HRESULT ( STDMETHODCALLTYPE *SetSite )( 
            IEmsExtensionModule * This,
            /* [in] */ IServiceProvider *pSP);
        
        DECLSPEC_XFGVIRT(IEmsExtensionModule, QueryClose)
        HRESULT ( STDMETHODCALLTYPE *QueryClose )( 
            IEmsExtensionModule * This,
            /* [retval][out] */ BOOL *pfCanClose);
        
        DECLSPEC_XFGVIRT(IEmsExtensionModule, Close)
        HRESULT ( STDMETHODCALLTYPE *Close )( 
            IEmsExtensionModule * This);
        
        DECLSPEC_XFGVIRT(IEmsExtensionModule, ModuleName)
        HRESULT ( STDMETHODCALLTYPE *ModuleName )( 
            IEmsExtensionModule * This,
            /* [retval][out] */ BSTR *pBstrName);
        
        DECLSPEC_XFGVIRT(IEmsExtensionModule, ModuleAuthor)
        HRESULT ( STDMETHODCALLTYPE *ModuleAuthor )( 
            IEmsExtensionModule * This,
            /* [retval][out] */ BSTR *pBstrAuthor);
        
        DECLSPEC_XFGVIRT(IEmsExtensionModule, ModuleDescription)
        HRESULT ( STDMETHODCALLTYPE *ModuleDescription )( 
            IEmsExtensionModule * This,
            /* [retval][out] */ BSTR *pBstrDescription);
        
        DECLSPEC_XFGVIRT(IEmsExtensionModule, ModuleGuid)
        HRESULT ( STDMETHODCALLTYPE *ModuleGuid )( 
            IEmsExtensionModule * This,
            /* [retval][out] */ GUID *pGuid);
        
        END_INTERFACE
    } IEmsExtensionModuleVtbl;

    interface IEmsExtensionModule
    {
        CONST_VTBL struct IEmsExtensionModuleVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEmsExtensionModule_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IEmsExtensionModule_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IEmsExtensionModule_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IEmsExtensionModule_SetSite(This,pSP)	\
    ( (This)->lpVtbl -> SetSite(This,pSP) ) 

#define IEmsExtensionModule_QueryClose(This,pfCanClose)	\
    ( (This)->lpVtbl -> QueryClose(This,pfCanClose) ) 

#define IEmsExtensionModule_Close(This)	\
    ( (This)->lpVtbl -> Close(This) ) 

#define IEmsExtensionModule_ModuleName(This,pBstrName)	\
    ( (This)->lpVtbl -> ModuleName(This,pBstrName) ) 

#define IEmsExtensionModule_ModuleAuthor(This,pBstrAuthor)	\
    ( (This)->lpVtbl -> ModuleAuthor(This,pBstrAuthor) ) 

#define IEmsExtensionModule_ModuleDescription(This,pBstrDescription)	\
    ( (This)->lpVtbl -> ModuleDescription(This,pBstrDescription) ) 

#define IEmsExtensionModule_ModuleGuid(This,pGuid)	\
    ( (This)->lpVtbl -> ModuleGuid(This,pGuid) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IEmsExtensionModule_INTERFACE_DEFINED__ */


#ifndef __IEmsHost_INTERFACE_DEFINED__
#define __IEmsHost_INTERFACE_DEFINED__

/* interface IEmsHost */
/* [object][uuid] */ 


EXTERN_C const IID IID_IEmsHost;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("AA41710A-EF62-4B56-A112-AFBA907B062C")
    IEmsHost : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE RegisterModule( 
            /* [in] */ IEmsExtensionModule *pModule) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IEmsHostVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEmsHost * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEmsHost * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEmsHost * This);
        
        DECLSPEC_XFGVIRT(IEmsHost, RegisterModule)
        HRESULT ( STDMETHODCALLTYPE *RegisterModule )( 
            IEmsHost * This,
            /* [in] */ IEmsExtensionModule *pModule);
        
        END_INTERFACE
    } IEmsHostVtbl;

    interface IEmsHost
    {
        CONST_VTBL struct IEmsHostVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEmsHost_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IEmsHost_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IEmsHost_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IEmsHost_RegisterModule(This,pModule)	\
    ( (This)->lpVtbl -> RegisterModule(This,pModule) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IEmsHost_INTERFACE_DEFINED__ */


#ifndef __IEmsMessagingTransport_INTERFACE_DEFINED__
#define __IEmsMessagingTransport_INTERFACE_DEFINED__

/* interface IEmsMessagingTransport */
/* [uuid] */ 


EXTERN_C const IID IID_IEmsMessagingTransport;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("28E6193B-C33D-4794-B1B5-EDE8B75B0748")
    IEmsMessagingTransport
    {
    public:
        BEGIN_INTERFACE
        virtual HRESULT STDMETHODCALLTYPE Send( void) = 0;
        
        END_INTERFACE
    };
    
    
#else 	/* C style interface */

    typedef struct IEmsMessagingTransportVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IEmsMessagingTransport, Send)
        HRESULT ( STDMETHODCALLTYPE *Send )( 
            IEmsMessagingTransport * This);
        
        END_INTERFACE
    } IEmsMessagingTransportVtbl;

    interface IEmsMessagingTransport
    {
        CONST_VTBL struct IEmsMessagingTransportVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEmsMessagingTransport_Send(This)	\
    ( (This)->lpVtbl -> Send(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IEmsMessagingTransport_INTERFACE_DEFINED__ */


#ifndef __IEmsService_INTERFACE_DEFINED__
#define __IEmsService_INTERFACE_DEFINED__

/* interface IEmsService */
/* [uuid] */ 


EXTERN_C const IID IID_IEmsService;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A2C467C9-0942-4961-A721-0007CA8F206A")
    IEmsService
    {
    public:
        BEGIN_INTERFACE
        END_INTERFACE
    };
    
    
#else 	/* C style interface */

    typedef struct IEmsServiceVtbl
    {
        BEGIN_INTERFACE
        
        END_INTERFACE
    } IEmsServiceVtbl;

    interface IEmsService
    {
        CONST_VTBL struct IEmsServiceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IEmsService_INTERFACE_DEFINED__ */


#ifndef __IEmsProcessService_INTERFACE_DEFINED__
#define __IEmsProcessService_INTERFACE_DEFINED__

/* interface IEmsProcessService */
/* [object][uuid] */ 


EXTERN_C const IID IID_IEmsProcessService;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("59B3382D-E46F-4A14-8CFA-0A36AA06D557")
    IEmsProcessService : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE TerminateProcess( 
            /* [in] */ DWORD dwPid) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IEmsProcessServiceVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEmsProcessService * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEmsProcessService * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEmsProcessService * This);
        
        DECLSPEC_XFGVIRT(IEmsProcessService, TerminateProcess)
        HRESULT ( STDMETHODCALLTYPE *TerminateProcess )( 
            IEmsProcessService * This,
            /* [in] */ DWORD dwPid);
        
        END_INTERFACE
    } IEmsProcessServiceVtbl;

    interface IEmsProcessService
    {
        CONST_VTBL struct IEmsProcessServiceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEmsProcessService_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IEmsProcessService_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IEmsProcessService_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IEmsProcessService_TerminateProcess(This,dwPid)	\
    ( (This)->lpVtbl -> TerminateProcess(This,dwPid) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IEmsProcessService_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_EmsServiceProvider;

#ifdef __cplusplus

class DECLSPEC_UUID("4B1D662B-7155-41A7-BA31-D13971E421D2")
EmsServiceProvider;
#endif

EXTERN_C const CLSID CLSID_EmsProcessService;

#ifdef __cplusplus

class DECLSPEC_UUID("9BBDFA08-D2C4-4FBA-A82A-6FBBDBAAE50D")
EmsProcessService;
#endif

EXTERN_C const CLSID CLSID_EmsHost;

#ifdef __cplusplus

class DECLSPEC_UUID("0F36A263-6823-4183-9DBA-BC44F881180B")
EmsHost;
#endif
#endif /* __EmsLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


