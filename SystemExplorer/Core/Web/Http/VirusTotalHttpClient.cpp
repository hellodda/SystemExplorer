#include "pch.h"
#include "VirusTotalHttpClient.h"

#include <winrt/Windows.Web.Http.h>
#include <winrt/Windows.Web.Http.Headers.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Data.Json.h>

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Data::Json;

namespace winrt::SystemExplorer::Core::Web::Http
{
	VirusTotalHttpClient::VirusTotalHttpClient(hstring const& apiKey)
		: HttpClientBase(L"https://www.virustotal.com/api/v3")
	{
		DefaultRequestHeaders().Append(L"x-apikey", apiKey);
	}

	IAsyncOperation<hstring> VirusTotalHttpClient::GetFileReportAsync(hstring const& analysisId)
	{
		try
		{
			Uri responseUri{ BaseUrl + L"/analyses/" + analysisId };

			auto response = co_await GetAsync(responseUri);
			response.EnsureSuccessStatusCode();
			
			auto responseContent = co_await response.Content().ReadAsStringAsync();

			if (!response.IsSuccessStatusCode())
			{
				co_return L"Error: " + hstring(std::to_wstring((int)response.StatusCode()));
			}

			auto jsonResponse = JsonObject::Parse(responseContent);

			co_return jsonResponse.GetNamedObject(L"data").GetNamedObject(L"attributes").GetNamedString(L"status").c_str();
		}
		catch (hresult_error const& hr)
		{
			co_return L"Exception occured";
		}
	}

	IAsyncOperation<hstring> VirusTotalHttpClient::ScanFileAsync(StorageFile const& file)
	{
		HttpMultipartFormDataContent multipartContent;

		auto stream = co_await file.OpenReadAsync();

		HttpStreamContent streamContent{ stream };
		multipartContent.Add(streamContent, L"file", file.Name());

		try
		{
			Uri requestUri{ BaseUrl + L"/files"};

			auto response = co_await PostAsync(requestUri, multipartContent);
			response.EnsureSuccessStatusCode();

			auto responseContent = co_await response.Content().ReadAsStringAsync();
			auto jsonResponse = JsonObject::Parse(responseContent);

			co_return jsonResponse.GetNamedObject(L"data").GetNamedString(L"id").c_str();
		}
		catch (hresult_error const& hr)
		{
			co_return L"";
		}
	}
}