#pragma once

namespace winrt::SystemExplorer::Converters
{
	using namespace winrt::Microsoft::UI::Xaml::Media::Imaging;

	struct HiconToBitmapSourceConverter
	{
		[[nodiscard]] static SoftwareBitmapSource Convert(_In_ HICON icon);
	};
}