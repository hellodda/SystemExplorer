;#ifndef SE_STATUS_H
;#define SE_STATUS_H

MessageIdTypedef=NTSTATUS

SeverityNames = (
    Success       = 0x0:STATUS_SEVERITY_SUCCESS
    Informational = 0x1:STATUS_SEVERITY_INFORMATIONAL
    Warning       = 0x2:STATUS_SEVERITY_WARNING
    Error         = 0x3:STATUS_SEVERITY_ERROR
)

FacilityNames = (
    System          = 0x0
    SystemExplorer  = 0x1:FACILITY_SE
    DynamicData     = 0x2:FACILITY_SE_DYNDATA
    KernelExplorer  = 0x3:FACILITY_KSE
)

MessageId    = 0x0001
Facility     = DynamicData
Severity     = Error
SymbolicName = STATUS_SE_DYNDATA_UNSUPPORTED_KERNEL
Language     = English
System Explorer dynamic data is not yet supported on this kernel version.
.

MessageId    = 0x0002
Facility     = DynamicData
Severity     = Error
SymbolicName = STATUS_SE_DYNDATA_VERSION_MISMATCH
Language     = English
System Explorer dynamic data version is incompatible.
.

MessageId    = 0x0003
Facility     = DynamicData
Severity     = Error
SymbolicName = STATUS_SE_DYNDATA_INVALID_LENGTH
Language     = English
System Explorer dynamic data is an invalid length.
.

MessageId    = 0x0004
Facility     = DynamicData
Severity     = Error
SymbolicName = STATUS_SE_DYNDATA_INVALID_SIGNATURE
Language     = English
System Explorer dynamic data signature is invalid.
.

MessageId    = 0x0001
Facility     = KernelExplorer
Severity     = Error
SymbolicName = STATUS_KSE_KSIDLL_VERSION_MISMATCH
Language     = English
Kernel Explorer kernel library version is incompatible.
.

;#endif
