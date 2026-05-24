#pragma once
#include <ShlObj.h>
#include <string>
#include <wil/resource.h>

namespace winrt::SystemExplorer::Helpers::Win32
{
    struct ShellHelper
    {
        [[nodiscard]] static wil::unique_hicon GetIconByIndex(int index, uint32_t sizeFlag = SHGFI_SMALLICON);
        [[nodiscard]] static int GetDefaultIconIndex();
        [[nodiscard]] static int GetIconIndex(const std::wstring& fileName);
    private:
        [[nodiscard]] static HIMAGELIST GetSystemImageList(uint32_t sizeFlag);
    };
}