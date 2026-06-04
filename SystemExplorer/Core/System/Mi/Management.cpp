#include "pch.h"
#include "Management.h"

#pragma comment(lib, "mi.lib")
#pragma comment(lib, "delayimp.lib")

namespace winrt::SystemExplorer::Core::System::Management
{
    inline void check_miresult(MI_Result result)
    {
        if (result == MI_RESULT_FAILED)
        {
            THROW_HR(E_FAIL);
        }
    }

    [[nodiscard]] static mi_variant mi_value_to_variant(MI_Type type, _In_ const MI_Value* value)
    {
        if (!value) return std::monostate{};

        switch (type)
        {
            case MI_BOOLEAN: return value->boolean != 0;
            case MI_SINT8:   return value->sint8;
            case MI_UINT8:   return value->uint8;
            case MI_SINT16:  return value->sint16;
            case MI_UINT16:  return value->uint16;
            case MI_SINT32:  return value->sint32;
            case MI_UINT32:  return value->uint32;
            case MI_SINT64:  return value->sint64;
            case MI_UINT64:  return value->uint64;
            case MI_REAL32:  return value->real32;
            case MI_REAL64:  return value->real64;
            case MI_STRING:  return winrt::hstring{ value->string };
            default:         return std::monostate{};
        }
    }

    ManagementClassObject::ManagementClassObject(_In_ const MI_Instance* instance)
    {
        if (instance)
        {
            check_miresult(MI_Instance_Clone(instance, instance_.put()));
        }
    }

    std::vector<std::pair<winrt::hstring, mi_variant>> ManagementClassObject::GetProperties() const
    {
        std::vector<std::pair<winrt::hstring, mi_variant>> properties;
        if (!instance_.is_valid()) return properties;

        MI_Uint32 count = 0;
        if (MI_Instance_GetElementCount(instance_.get(), &count) != MI_RESULT_OK) return properties;

        for (MI_Uint32 i = 0; i < count; ++i)
        {
            const MI_Char* name{ nullptr };
            MI_Value value{};
            MI_Type type{};
            MI_Uint32 flags{ 0 };

            if (MI_Instance_GetElementAt(instance_.get(), i, &name, &value, &type, &flags) == MI_RESULT_OK)
            {
                if (name && name[0] != L'_')
                {
                    if (flags & MI_FLAG_NULL)
                    {
                        properties.emplace_back(name, std::monostate{});
                    }
                    else
                    {
                        properties.emplace_back(name, mi_value_to_variant(type, &value));
                    }
                }
            }
        }
        return properties;
    }

    std::pair<winrt::hstring, mi_variant> ManagementClassObject::GetProperty(winrt::hstring const& name) const
    {
        if (!instance_.is_valid()) return { name, std::monostate{} };

        MI_Value value;
        MI_Type type;
        MI_Uint32 flags{ 0 };

        if (MI_Instance_GetElement(instance_.get(), name.c_str(), &value, &type, &flags, nullptr) == MI_RESULT_OK)
        {
            return { name, mi_value_to_variant(type, &value) };
        }
        return { name, std::monostate{} };
    }

    struct async_state
    {
        concurrency::task_completion_event<std::vector<ManagementClassObject>> tce;
        std::vector<ManagementClassObject> results;
        unique_mi_operation operation{}; 
    };

    void MI_CALL QueryCallback(
        _In_opt_ MI_Operation* operation,
        _In_opt_ PVOID callbackContext,
        _In_opt_ CONST MI_Instance* instance,
        _In_ MI_Boolean moreResults,
        _In_ MI_Result resultCode,
        _In_opt_ CONST MI_Char* errorMessage,
        _In_opt_ CONST MI_Instance* errorDetails,
        _In_opt_ MI_Result(MI_CALL* resultAcknowledgement)(MI_Operation* operation))
    {
        auto state = static_cast<async_state*>(callbackContext);
        if (!state) return;

        if (instance)
        {
            state->results.emplace_back(instance);
        }

        if (resultAcknowledgement && moreResults)
        {
            resultAcknowledgement(operation);
        }

        if (!moreResults)
        {
            state->operation.reset();

            if (resultCode == MI_RESULT_OK)
                state->tce.set(std::move(state->results));
            else
                state->tce.set_exception(std::make_exception_ptr(std::runtime_error("ratio failed")));

            delete state;
        }
    }

    void ManagementConnection::initialize()
    {
        app_.reset(new MI_Application{});
        check_miresult(MI_Application_InitializeV1(0, nullptr, nullptr, app_.get()));

        auto options = std::make_unique<MI_DestinationOptions>();
        check_miresult(MI_Application_NewDestinationOptions(app_.get(), options.get()));

        if (!user_.empty())
        {
            MI_UserCredentials creds{};
            creds.authenticationType = MI_AUTH_TYPE_DEFAULT;
            creds.credentials.usernamePassword.domain = const_cast<MI_Char*>(L"");
            creds.credentials.usernamePassword.username = const_cast<MI_Char*>(user_.c_str());
            creds.credentials.usernamePassword.password = const_cast<MI_Char*>(password_.c_str());

            check_miresult(MI_DestinationOptions_AddDestinationCredentials(options.get(), &creds));
        }

        session_.reset(new MI_Session{});
        check_miresult(MI_Application_NewSession(app_.get(), nullptr, nullptr, options.get(), nullptr, nullptr, session_.get()));
    }

    concurrency::task<std::vector<ManagementClassObject>> ManagementConnection::ExecuteQueryAsync(winrt::hstring const& query)
    {
        if (!session_) [[unlikely]]
            THROW_HR_MSG(E_POINTER, "Session is nullptr.");

        auto state = new async_state();
        state->operation.reset(new MI_Operation{});
        auto task = concurrency::task<std::vector<ManagementClassObject>>(state->tce);

        MI_OperationCallbacks callbacks{ MI_OPERATIONCALLBACKS_NULL };
        callbacks.callbackContext = state;
        callbacks.instanceResult = QueryCallback;

        MI_Session_QueryInstances(
            session_.get(),
            0,
            NULL,
            connectionNamespace_.c_str(),
            L"WQL",
            query.c_str(),
            &callbacks,
            state->operation.get()
        );
        return task;
    }
}