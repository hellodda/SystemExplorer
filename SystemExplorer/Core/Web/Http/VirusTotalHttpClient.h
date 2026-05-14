#pragma once
#include "Base.h"

namespace winrt::SystemExplorer::Core::Web::Http
{
	using namespace winrt::Windows::Web::Http;

	struct VirusTotalHttpClient : HttpClientBase
	{
		VirusTotalHttpClient(
			hstring const& baseUrl,
			hstring const& apiKey
		);	

		IAsyncOperation<hstring> ScanFileAsync(StorageFile const& file);
	};
}

