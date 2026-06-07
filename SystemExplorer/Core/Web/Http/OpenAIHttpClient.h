#pragma once
#include "Base.h"
#include <winrt/Windows.Data.Json.h>

namespace winrt::SystemExplorer::Core::Web::Http
{
    using namespace winrt::Windows::Foundation;

    namespace NetIoInteligenceModels
    {
        constexpr const wchar_t* DeepSeek_R1_0528 = L"deepseek-ai/DeepSeek-R1-0528";
        constexpr const wchar_t* Llama_4_Maverick_17B_128E_Instruct_FP8 = L"meta-llama/Llama-4-Maverick-17B-128E-Instruct-FP8";
        constexpr const wchar_t* Meta_llama_3_dot_1_70b_instruct = L"gpt-4o-mini";
        /*constexpr const wchar_t* Qwen3_dot_6_35B_A3B = L"Qwen3.6-35B-A3B";
        constexpr const wchar_t* Gpt_oss_20b = L"gpt-oss-20b";
        constexpr const wchar_t* Gemma_4_26b_a4b_it = L"gemma-4-26b-a4b-it";
        constexpr const wchar_t* DeepSeek_V3 = L"deepseek-ai/DeepSeek-V3";
        constexpr const wchar_t* Llama_3_3_70B_Instruct = L"meta-llama/Llama-3.3-70B-Instruct";
        constexpr const wchar_t* Gpt_oss_120b = L"gpt-oss-120b";
        constexpr const wchar_t* Qwen_2_5_72B_Instruct = L"Qwen/Qwen2.5-72B-Instruct";
        constexpr const wchar_t* Glm_4_5_Air = L"glm-4.5-air";
        constexpr const wchar_t* Glm_4_9b_Chat = L"glm-4-9b-chat";
        constexpr const wchar_t* Mistral_Large_Instruct = L"mistralai/Mistral-Large-Instruct";*/
    }

    struct OpenAIHttpClient : HttpClientBase
    {
        OpenAIHttpClient(hstring const& apiKey);

        void SetInteligenceModel(hstring const& modelName);
        void SetSystemSetting(hstring const& setting);

        void IsChatHistoryEnabled(bool isEnabled);
        bool IsChatHistoryEnabled() const;
        void ClearChatHistory();

        [[nodiscard]] IAsyncOperation<hstring> SendPromptAsync(hstring const& propmpt);
    private:
        [[nodiscard]] hstring parseJsonResponse(hstring const& content);
    private:
        hstring apiKey_{};
        hstring systemSetting_{};
        hstring inteligenceModelName_{ NetIoInteligenceModels::Meta_llama_3_dot_1_70b_instruct };

        bool isChatHistoryEnabled_{ false };
        winrt::Windows::Data::Json::JsonArray chatHistory_{ nullptr };
    };
}
