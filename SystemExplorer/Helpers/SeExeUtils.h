#pragma once

extern "C" {
    WINBASEAPI LONG WINAPI GetCurrentPackageId(UINT32* bufferLength, BYTE* buffer);
}

#ifdef WINRT_Windows_ApplicationModel_H
inline bool IsPackaged()
{
    static const auto isPackaged = []() {
        UINT32 bufferLength = 0;
        const auto hr = GetCurrentPackageId(&bufferLength, nullptr);
        return hr != APPMODEL_ERROR_NO_PACKAGE;
        }();
    return isPackaged;
}
#endif
