#include "pch.h"
#include "VirusTotalHttpClient.h"
#include <winrt/Windows.Web.Http.Headers.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Data.Json.h>

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Data::Json;

namespace winrt::SystemExplorer::Core::Web::Http
{
	VirusTotalHttpClient::VirusTotalHttpClient(hstring const& baseUrl, hstring const& apiKey)
		: HttpClientBase(baseUrl)
	{
		DefaultRequestHeaders().Append(L"X-Apikey", apiKey);
	}

	IAsyncOperation<hstring> VirusTotalHttpClient::ScanFileAsync(StorageFile const& file)
	{
		HttpMultipartFormDataContent multipartContent;

		auto stream = co_await file.OpenReadAsync();

		HttpStreamContent streamContent{ stream };
		multipartContent.Add(streamContent, L"file", file.Name());

		try
		{
			Uri requestUri{ baseUrl_ + L"/files"};

			auto response = co_await PostAsync(requestUri, multipartContent);
			auto responseContent = co_await response.Content().ReadAsStringAsync();
			auto jsonResponse = JsonObject::Parse(responseContent);

			co_return jsonResponse.GetNamedObject(L"data").GetNamedString(L"id").c_str();
		}
		catch (hresult_error const&)
		{
			co_return L"";
		}
	}
}