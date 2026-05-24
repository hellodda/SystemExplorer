#pragma once

#include "Core/Settings/AppState.g.h"
#include <Core/Serialization/SettingsBase.h>
#include <factory.h>
#include <property.h>

namespace winrt::SystemExplorer::Core::Settings::implementation
{
    struct AppState : AppStateT<AppState, Serialization::implementation::SettingsBase>
    {
        static Settings::AppState Instance()
        {
            static auto instance = make<AppState>();
            return instance;
        }

        [[nodiscard]] int32_t ProcessesNameColumnLastWidth() { return Get(L"ProcessesNameColumnLastWidth", int32_t{ 250 }); }
        void ProcessesNameColumnLastWidth(int32_t value) { Set(L"ProcessesNameColumnLastWidth", value); }

        [[nodiscard]] int32_t ProcessesStatusColumnLastWidth() { return Get(L"ProcessesStatusColumnLastWidth", int32_t{ 50 }); }
        void ProcessesStatusColumnLastWidth(int32_t value) { Set(L"ProcessesStatusColumnLastWidth", value); }

        [[nodiscard]] int32_t ProcessesPidColumnLastWidth() { return Get(L"ProcessesPidColumnLastWidth", int32_t{ 50 }); }
        void ProcessesPidColumnLastWidth(int32_t value) { Set(L"ProcessesPidColumnLastWidth", value); }

        [[nodiscard]] int32_t ProcessesCpuUsageColumnLastWidth() { return Get(L"ProcessesCpuUsageColumnLastWidth", int32_t{ 75 }); }
        void ProcessesCpuUsageColumnLastWidth(int32_t value) { Set(L"ProcessesCpuUsageColumnLastWidth", value); }

        [[nodiscard]] int32_t ProcessesIoRateColumnLastWidth() { return Get(L"ProcessesIoRateColumnLastWidth", int32_t{ 75 }); }
        void ProcessesIoRateColumnLastWidth(int32_t value) { Set(L"ProcessesIoRateColumnLastWidth", value); }

        [[nodiscard]] int32_t ProcessesPrivateBytesLastWidth() { return Get(L"ProcessesPrivateBytesLastWidth", int32_t{ 50 }); }
        void ProcessesPrivateBytesLastWidth(int32_t value) { Set(L"ProcessesPrivateBytesLastWidth", value); }

        NOTIFYING_SETTING(int32_t, ItemFontSize, 13);
        NOTIFYING_SETTING(int32_t, ItemIconSize, 18);
    };
}
namespace winrt::SystemExplorer::Core::Settings::factory_implementation {
    struct AppState : AppStateT<AppState, implementation::AppState> {};
};