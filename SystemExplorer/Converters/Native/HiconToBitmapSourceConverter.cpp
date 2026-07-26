#include "pch.h"
#include "winrt_module_imports.h"
#include "HiconToBitmapSourceConverter.h"

#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>
#include <winrt/Windows.Graphics.Imaging.h>

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Windowscodecs.lib")

namespace winrt::SystemExplorer::Converters::Native
{
	SoftwareBitmapSource HiconToBitmapSourceConverter::Convert(wil::unique_hicon icon)
	{
		if (!icon.is_valid()) return nullptr; 

		std::call_once(initializeFlag_, []()
		{
			initialize();
		});
		
		com_ptr<IWICBitmap> wicBitmap;
		THROW_IF_FAILED(wicImagingFactory_->CreateBitmapFromHICON(
			icon.get(),
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
		wicImagingFactory_ = winrt::create_instance<IWICImagingFactory>(
			CLSID_WICImagingFactory,
			CLSCTX_INPROC_SERVER
		);

		softwareBitmapNativeFactory_ = winrt::create_instance<ISoftwareBitmapNativeFactory>(
			CLSID_SoftwareBitmapNativeFactory,
			CLSCTX_INPROC_SERVER
		);
	}
}