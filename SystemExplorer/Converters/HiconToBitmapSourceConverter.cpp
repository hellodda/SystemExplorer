#include "pch.h"
#include "HiconToBitmapSourceConverter.h"

#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <windows.graphics.imaging.interop.h>
#include <wincodec.h>
#include <Shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Windowscodecs.lib")

using namespace winrt::Windows::Graphics::Imaging;

namespace winrt::SystemExplorer::Converters
{
	SoftwareBitmapSource HiconToBitmapSourceConverter::Convert(HICON icon)
	{
		if (!icon) return nullptr; 

		com_ptr<IWICImagingFactory> wicImagingFactory;
		THROW_IF_FAILED(CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&wicImagingFactory)
		));

		com_ptr<IWICBitmap> wicBitmap;
		THROW_IF_FAILED(wicImagingFactory->CreateBitmapFromHICON(
			icon,
			wicBitmap.put()
		));

		com_ptr<ISoftwareBitmapNativeFactory> softwareBitmapNativeFactory;
		THROW_IF_FAILED(CoCreateInstance(
			CLSID_SoftwareBitmapNativeFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&softwareBitmapNativeFactory)
		));

		SoftwareBitmap softwareBitmap{ nullptr };
		THROW_IF_FAILED(softwareBitmapNativeFactory->CreateFromWICBitmap(
			wicBitmap.get(),
			FALSE,
			guid_of<SoftwareBitmap>(),
			put_abi(softwareBitmap)
		));
		if (softwareBitmap.BitmapPixelFormat() != BitmapPixelFormat::Bgra8 ||
			softwareBitmap.BitmapAlphaMode() != BitmapAlphaMode::Premultiplied)
		{
			softwareBitmap = SoftwareBitmap::Convert(
				softwareBitmap,
				BitmapPixelFormat::Bgra8,
				BitmapAlphaMode::Premultiplied
			);
		}
		SoftwareBitmapSource source;
		source.SetBitmapAsync(softwareBitmap);

		return source;
	}
}