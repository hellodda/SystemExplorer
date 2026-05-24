#pragma once
#include <wincodec.h>
#include <windows.graphics.imaging.interop.h>

namespace winrt::SystemExplorer::Converters::Native
{
	using namespace winrt::Microsoft::UI::Xaml::Media::Imaging;
	using namespace winrt::Windows::Graphics::Imaging;
	using namespace winrt::Windows::Foundation;

	struct HiconToBitmapSourceConverter
	{
		[[nodiscard]] static SoftwareBitmapSource Convert(wil::unique_hicon icon);
	private:
		static void initialize();
	private:
		static inline com_ptr<IWICImagingFactory> wicImagingFactory_{ nullptr };
		static inline com_ptr<ISoftwareBitmapNativeFactory> softwareBitmapNativeFactory_{ nullptr };
		static inline std::once_flag initializeFlag_{};
	};
}