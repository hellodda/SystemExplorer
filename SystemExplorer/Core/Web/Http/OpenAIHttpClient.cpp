#include "pch.h"
#include "OpenAIHttpClient.h" 
#include <winrt/Windows.Web.Http.h>
#include <winrt/Windows.Web.Http.Headers.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Streams.h>

using namespace winrt::Windows::Web::Http;
using namespace winrt::Windows::Data::Json;
using namespace winrt::Windows::Foundation;

namespace winrt::SystemExplorer::Core::Web::Http
{
    OpenAIHttpClient::OpenAIHttpClient(hstring const& apiKey)
        : HttpClientBase(L"https://models.inference.ai.azure.com/chat/completions")
    {
        DefaultRequestHeaders().Authorization(
            Headers::HttpCredentialsHeaderValue(L"Bearer", apiKey)
        );
        DefaultRequestHeaders().Append(L"Accept", L"application/json");
        chatHistory_ = JsonArray();
    }

    void OpenAIHttpClient::SetInteligenceModel(hstring const& modelName)
    {
        inteligenceModelName_ = modelName;
    }

    void OpenAIHttpClient::SetSystemSetting(hstring const& setting)
    {
        systemSetting_ = setting;
    }

    void OpenAIHttpClient::IsChatHistoryEnabled(bool isEnabled)
    {
        isChatHistoryEnabled_ = isEnabled;
    }

    bool OpenAIHttpClient::IsChatHistoryEnabled() const
    {
        return isChatHistoryEnabled_;
    }

    void OpenAIHttpClient::ClearChatHistory()
    {
        chatHistory_.Clear();
    }

    IAsyncOperation<hstring> OpenAIHttpClient::SendPromptAsync(hstring const& propmpt)
    {
        JsonObject requestBody;
        requestBody.SetNamedValue(L"model", JsonValue::CreateStringValue(inteligenceModelName_));

        JsonArray messages;
        JsonObject systemMessage;
        systemMessage.SetNamedValue(L"role", JsonValue::CreateStringValue(L"system"));
        systemMessage.SetNamedValue(L"content", JsonValue::CreateStringValue(systemSetting_));
        messages.Append(systemMessage);

        if (isChatHistoryEnabled_)
        {
            for (auto const& msg : chatHistory_)
            {
                messages.Append(msg);
            }
        }

        JsonObject userMessage;
        userMessage.SetNamedValue(L"role", JsonValue::CreateStringValue(L"user"));
        userMessage.SetNamedValue(L"content", JsonValue::CreateStringValue(propmpt));
        messages.Append(userMessage);

        requestBody.SetNamedValue(L"messages", messages);

        HttpStringContent content(
            requestBody.Stringify(),
            Windows::Storage::Streams::UnicodeEncoding::Utf8,
            L"application/json"
        );

        auto response = co_await PostAsync(Uri{ BaseUrl }, content);

        try
        {
            response.EnsureSuccessStatusCode();
        }
        catch (hresult_error const& hr)
        {

        }
        CATCH_LOG()

        hstring rawResponse = co_await response.Content().ReadAsStringAsync();
        hstring parsedResponse = parseJsonResponse(rawResponse);

        if (isChatHistoryEnabled_ && !parsedResponse.empty())
        {
            chatHistory_.Append(userMessage);
            JsonObject assistantMessage;
            assistantMessage.SetNamedValue(L"role", JsonValue::CreateStringValue(L"assistant"));
            assistantMessage.SetNamedValue(L"content", JsonValue::CreateStringValue(parsedResponse));
            chatHistory_.Append(assistantMessage);
        }

        co_return parsedResponse;
    }

    hstring OpenAIHttpClient::parseJsonResponse(hstring const& content)
    {
        auto jsonResponse = JsonObject::Parse(content);
        auto choices = jsonResponse.GetNamedArray(L"choices");
        auto firstChoice = choices.GetObjectAt(0);
        auto message = firstChoice.GetNamedObject(L"message");
        return message.GetNamedString(L"content");
    }
}