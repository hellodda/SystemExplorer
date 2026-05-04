#include "pch.h"
#include "HiconToBitmapSourceConverter.h"

#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>
#include <winrt/Windows.Graphics.Imaging.h>

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Windowscodecs.lib")

namespace winrt::SystemExplorer::Converters
{
	HiconToBitmapSourceConverter::HiconToBitmapSourceConverter()
	{
		initialize();
	}
	SoftwareBitmapSource HiconToBitmapSourceConverter::Convert(HICON icon)
	{
		if (!icon) return nullptr; 
		
		com_ptr<IWICBitmap> wicBitmap;
		THROW_IF_FAILED(wicImagingFactory_->CreateBitmapFromHICON(
			icon,
			wicBitmap.put()
		));

		SoftwareBitmap softwareBitmap{ nullptr };
		THROW_IF_FAILED(softwareBitmapNativeFactory_->CreateFromWICBitmap(
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
	void HiconToBitmapSourceConverter::initialize()
	{
		THROW_IF_FAILED(CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&wicImagingFactory_)
		));

		THROW_IF_FAILED(CoCreateInstance(
			CLSID_SoftwareBitmapNativeFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&softwareBitmapNativeFactory_)
		));
	}
}