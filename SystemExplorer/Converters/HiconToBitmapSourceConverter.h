#pragma once
#include <wincodec.h>
#include <windows.graphics.imaging.interop.h>

namespace winrt::SystemExplorer::Converters
{
	using namespace winrt::Microsoft::UI::Xaml::Media::Imaging;
	using namespace winrt::Windows::Graphics::Imaging;

	struct HiconToBitmapSourceConverter
	{
		HiconToBitmapSourceConverter();

		[[nodiscard]] SoftwareBitmapSource Convert(_In_ HICON icon);
	private:
		void initialize();
	private:
		com_ptr<IWICImagingFactory> wicImagingFactory_;
		com_ptr<ISoftwareBitmapNativeFactory> softwareBitmapNativeFactory_;
	};
}