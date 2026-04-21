#include "pch.h"
#include "AppLanguageItem.h"
#if __has_include("Models/Items/AppLanguageItem.g.cpp")
#include "Models/Items/AppLanguageItem.g.cpp"
#endif
#include <winrt/Windows.Globalization.h>
#include <Helpers/StringsHelper.h>

using namespace winrt::Windows::Globalization;
using namespace winrt::SystemExplorer::Helpers;

namespace winrt::SystemExplorer::Models::Items::implementation
{
	AppLanguageItem::AppLanguageItem(hstring const& code, bool systemDefault)
	{
		if (systemDefault || code.empty())
		{
			Code = ApplicationLanguages::Languages().GetAt(0);
			Name = StringsHelper::SystemTheme();
		}
		else
		{
			Language language{ code };

			Code = language.LanguageTag();
			Name = language.NativeName();
		}
	}
}
