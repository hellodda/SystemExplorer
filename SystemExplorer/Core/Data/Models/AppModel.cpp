#include "pch.h"
#include "winrt_module_imports.h"
#include "AppModel.h"
#if __has_include("Core/Data/Models/AppModel.g.cpp")
#include "Core/Data/Models/AppModel.g.cpp"
#endif
#include <App.xaml.h>

namespace winrt::SystemExplorer::Core::Data::Models::implementation
{
	float AppModel::AppWindowDPI() noexcept
	{
		if (AppWindowDPI_ == 0.0f)
		{
			auto dpi = GetDpiForWindow((HWND)CurrentApplication::Window().Hwnd());
			AppWindowDPI_ = dpi > 0 ? dpi / 96.0f : 1.0f;
		}
		return AppWindowDPI_;
	}
}
