#include "pch.h"
#include "ActionStatusToBrushConverter.h"
#if __has_include("Converters/ActionStatusToBrushConverter.g.cpp")
#include "Converters/ActionStatusToBrushConverter.g.cpp"
#endif
#include "../Models/ActionItem.h"

namespace winrt::SystemExplorer::Converters::implementation
{
	IInspectable ActionStatusToBrushConverter::Convert(IInspectable const& value, TypeName const&, IInspectable const&, hstring const&)
	{
        if (!value)
            return nullptr;

        auto status = unbox_value<SystemExplorer::Models::ActionStatus>(value);

        switch (status)
        {
        case SystemExplorer::Models::ActionStatus::Success:
            return Application::Current().Resources().Lookup(box_value(L"SystemFillColorSuccessBrush"));

        case SystemExplorer::Models::ActionStatus::Warning:
            return Application::Current().Resources().Lookup(box_value(L"SystemFillColorCautionBrush"));

        case SystemExplorer::Models::ActionStatus::Failure:
            return Application::Current().Resources().Lookup(box_value(L"SystemFillColorCriticalBrush"));
        default:
            return Application::Current().Resources().Lookup(box_value(L"SystemControlBackgroundBaseLowBrush"));
        }
	}
	IInspectable ActionStatusToBrushConverter::ConvertBack(IInspectable const&, TypeName const&, IInspectable const&, hstring const&)
	{
		throw hresult_not_implemented();
	}
}
