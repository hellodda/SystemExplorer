#pragma once
#include <winrt/Windows.Web.Http.h>

namespace winrt::SystemExplorer::Core::Web::Http
{
	struct HttpClientBase : Windows::Web::Http::HttpClient
	{
		HttpClientBase(hstring const& baseUrl)
			: BaseUrl(baseUrl) {}

		hstring BaseUrl;
	};
}