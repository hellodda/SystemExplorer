#include "pch.h"
#include "ShellHelper.h"

namespace winrt::SystemExplorer::Helpers::Win32
{
    HIMAGELIST ShellHelper::GetSystemImageList(uint32_t sizeFlag)
    {
        if (sizeFlag == SHGFI_SMALLICON)
        {
            static HIMAGELIST smallList = []()
            {
                SHFILEINFOW sfi{};
                return (HIMAGELIST)SHGetFileInfoW(L".exe", FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(sfi),
                    SHGFI_USEFILEATTRIBUTES | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
            }();
            return smallList;
        }
        if (sizeFlag == SHGFI_LARGEICON)
        {
            static HIMAGELIST largeList = []() 
            {
                SHFILEINFOW sfi{};
                return (HIMAGELIST)SHGetFileInfoW(L".exe", FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(sfi),
                    SHGFI_USEFILEATTRIBUTES | SHGFI_SYSICONINDEX | SHGFI_LARGEICON);
            }();
            return largeList;
        }

        SHFILEINFOW sfi{};
        return (HIMAGELIST)SHGetFileInfoW(L".exe", FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(sfi),
            SHGFI_USEFILEATTRIBUTES | SHGFI_SYSICONINDEX | sizeFlag);
    }

    wil::unique_hicon ShellHelper::GetIconByIndex(int index, uint32_t sizeFlag)
    {
        HIMAGELIST himl = GetSystemImageList(sizeFlag);
        if (!himl) return nullptr;

        return wil::unique_hicon{ ImageList_GetIcon(himl, index, ILD_TRANSPARENT) };
    }

    int ShellHelper::GetDefaultIconIndex()
    {
        static int defaultIndex = []()
        {
            SHFILEINFOW sfi{};
            if (SHGetFileInfoW(L".exe", FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(sfi),
                SHGFI_USEFILEATTRIBUTES | SHGFI_SYSICONINDEX | SHGFI_SMALLICON))
            {
                return sfi.iIcon;
            }
            return 0; 
        }();

        return defaultIndex;
    }

    int ShellHelper::GetIconIndex(const std::wstring& fileName)
    {
        SHFILEINFOW sfi{};

        if (SHGetFileInfoW(fileName.c_str(), FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(sfi),
            SHGFI_USEFILEATTRIBUTES | SHGFI_SYSICONINDEX | SHGFI_SMALLICON))
        {
            return sfi.iIcon;
        }

        return GetDefaultIconIndex();
    }
}