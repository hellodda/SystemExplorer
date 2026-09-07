using System.Runtime.InteropServices;

namespace ManagedModule.Interop;
public enum EMS_MESSAGE_ID : int
{
    InvalidEmsMessageId = 0,
    EmsMessageIdRegisterModule,
    EmsMessageIdContent,
    MaxEmsMessageId
}
public enum EMS_API_MESSAGE_TRANSPORT_TYPE : int
{
    UnknownEmsApiMessageTransport = 0,
    EmsApiMessagePipeTransport,
    EmsApiMessageAlpcTransport,
    MaxEmsApiMessageTransport
}

[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
public struct EMSM_REGISTER_MODULE
{
    public int Status; 
    public Guid ModuleGuid;

    [MarshalAs(UnmanagedType.LPWStr)]
    public string ModuleName;

    [MarshalAs(UnmanagedType.LPWStr)]
    public string ModuleAuthor;

    [MarshalAs(UnmanagedType.LPWStr)]
    public string ModuleDescription;
}

[StructLayout(LayoutKind.Sequential)]
public struct EMS_API_MESSAGE_HEADER
{
    public short ApiVersion;
    public EMS_API_MESSAGE_TRANSPORT_TYPE TransportType;
    public EMS_MESSAGE_ID MessageId;
}

[StructLayout(LayoutKind.Sequential)]
public struct EMS_API_MESSAGE_MODULE
{
    public EMSM_REGISTER_MODULE RegisterModule;
}

[StructLayout(LayoutKind.Sequential)]
public struct EMS_API_MESSAGE
{
    public EMS_API_MESSAGE_HEADER Header;
    public EMS_API_MESSAGE_MODULE Module;
}

public static class EmsNativeApi
{
    private const string EMS_DLL_NAME = "ModularSystem.dll";

    [DllImport(EMS_DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int EmsConnectToHost(
        out IntPtr ConnectionHandle,
        ref EMSM_REGISTER_MODULE ModuleRegistartionMessage
    );

    [DllImport(EMS_DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int EmsSendMessage(
        IntPtr ConnectionHandle,
        ref EMS_API_MESSAGE Message
    );

    [DllImport(EMS_DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int EmsValidateMessage(
        ref EMS_API_MESSAGE Message
    );

    [DllImport(EMS_DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern void EmsInitMessage(
        out EMS_API_MESSAGE Message,
        EMS_MESSAGE_ID MessageId
    );
}