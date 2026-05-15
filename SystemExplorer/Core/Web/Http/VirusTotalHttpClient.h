#pragma once
#include "Base.h"

#include <winrt/Windows.Storage.h>

namespace winrt::SystemExplorer::Core::Web::Http
{
	using namespace winrt::Windows::Foundation;
	using namespace winrt::Windows::Storage;
	using namespace winrt::Windows::Web::Http;

	struct VirusTotalHttpClient : HttpClientBase
	{
		VirusTotalHttpClient(
			hstring const& apiKey
		);	

		IAsyncOperation<hstring> GetFileReportAsync(hstring const& id);
		IAsyncOperation<hstring> ScanFileAsync(StorageFile const& file);
	};
}

