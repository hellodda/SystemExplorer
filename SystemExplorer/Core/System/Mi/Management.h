#pragma once

#include <mi.h>
#include <variant>
#include "../../../../Common/wilx.h"

namespace winrt::SystemExplorer::Core::System::Management
{
    struct unique_mi_instance : wilx::unique_any<MI_Instance_Delete>
    {
        [[nodiscard]] unique_mi_instance clone() const
        {
            unique_mi_instance cloned;
            if (is_valid())
            {
                if (MI_Instance_Clone(get(), cloned.put()) != MI_RESULT_OK)
                {
                    throw winrt::hresult_error{};
                }
            }
            return cloned;
        }
    };

    namespace details
    {
        struct mi_application_deleter 
        {
            void operator()(MI_Application* ptr) const
            {
                if (ptr) {
                    if (ptr->reserved1 != 0) MI_Application_Close(ptr);
                    delete ptr;
                }
            }
        };

        struct mi_session_deleter
        {
            void operator()(MI_Session* ptr) const
            {
                if (ptr) {
                    if (ptr->reserved1 != 0) MI_Session_Close(ptr, nullptr, nullptr);
                    delete ptr;
                }
            }
        };

        struct mi_operation_deleter
        {
            void operator()(MI_Operation* ptr) const
            {
                if (ptr) {
                    if (ptr->reserved1 != 0) MI_Operation_Close(ptr);
                    delete ptr;
                }
            }
        };

        struct mi_destination_options_deleter
        {
            void operator()(MI_DestinationOptions* ptr) const 
            {
                if (ptr)
                {
                    if (ptr->reserved1 != 0) MI_DestinationOptions_Delete(ptr);
                    delete ptr;
                }
            }
        };

    }
    using unique_mi_application = std::unique_ptr<MI_Application, details::mi_application_deleter>;
    using unique_mi_session = std::unique_ptr<MI_Session, details::mi_session_deleter>;
    using unique_mi_operation = std::unique_ptr<MI_Operation, details::mi_operation_deleter>;
    using unique_mi_destination_options = std::unique_ptr<MI_DestinationOptions, details::mi_destination_options_deleter>;

    using mi_variant = std::variant
    <
        std::monostate,
        bool,
        int8_t,
        uint8_t,
        int16_t,
        uint16_t,
        int32_t,
        uint32_t,
        int64_t,
        uint64_t,
        float,
        double,
        winrt::hstring
    >;

    struct ManagementClassObject
    {
        ManagementClassObject() = default;
        ManagementClassObject(std::nullptr_t) {}
        explicit ManagementClassObject(_In_ const MI_Instance* instance);

        ManagementClassObject(const ManagementClassObject& other) : instance_(other.instance_.clone()) {}
        ManagementClassObject& operator=(const ManagementClassObject& other)
        {
            if (this != &other)
                instance_ = other.instance_.clone();
            return *this;
        }

        ManagementClassObject(ManagementClassObject&& other) noexcept = default;
        ManagementClassObject& operator=(ManagementClassObject&& other) noexcept = default;

        [[nodiscard]] std::vector<std::pair<winrt::hstring, mi_variant>> GetProperties() const;
        [[nodiscard]] std::pair<winrt::hstring, mi_variant> GetProperty(winrt::hstring const& name) const;

    private:
        unique_mi_instance instance_{};
    };

    struct ManagementConnection
    {
        ManagementConnection(winrt::hstring const& connectionNamespace)
            : connectionNamespace_(connectionNamespace)
        {
            initialize();
        }

        ManagementConnection(
            winrt::hstring const& connectionNamespace,
            winrt::hstring const& user,
            winrt::hstring const& password
        )
            : connectionNamespace_(connectionNamespace),
            user_(user), password_(password)
        {
            initialize();
        }

        ~ManagementConnection() = default; 

        [[nodiscard]] concurrency::task<std::vector<ManagementClassObject>> ExecuteQueryAsync(winrt::hstring const& query);

    private:
        void initialize();
    private:
        unique_mi_application app_{};
        unique_mi_session session_{};

        winrt::hstring connectionNamespace_{};
        winrt::hstring user_{};
        winrt::hstring password_{};
    };
}