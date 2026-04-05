#include "pch.h"
#include "AppThemeResourceFactory.h"

namespace winrt::SystemExplorer::Data::Factories
{
	IObservableVector<SystemExplorer::Models::Items::AppThemeResourceItem> winrt::SystemExplorer::Data::Factories::AppThemeResourceFactory::AppThemeResources()
	{
        auto resources = single_threaded_observable_vector<SystemExplorer::Models::Items::AppThemeResourceItem>();

        resources.Append(SystemExplorer::Models::Items::AppThemeResourceItem{ L"Default", L"#00000000" });
        resources.Append(SystemExplorer::Models::Items::AppThemeResourceItem{ L"Yellow Gold", L"#32FFB900" });
        resources.Append(SystemExplorer::Models::Items::AppThemeResourceItem{ L"Orange Bright", L"#32F7630C" });
        resources.Append(SystemExplorer::Models::Items::AppThemeResourceItem{ L"Brick Red", L"#32D13438" });
        resources.Append(SystemExplorer::Models::Items::AppThemeResourceItem{ L"Mod Red", L"#32FF4343" });
        resources.Append(SystemExplorer::Models::Items::AppThemeResourceItem{ L"Red", L"#32EA005E" });
        resources.Append(SystemExplorer::Models::Items::AppThemeResourceItem{ L"Rose Bright", L"#32EA005E" });
        resources.Append(SystemExplorer::Models::Items::AppThemeResourceItem{ L"Blue", L"#320078D7" });
        resources.Append(SystemExplorer::Models::Items::AppThemeResourceItem{ L"Iris Pastel", L"#328764B8" });
        resources.Append(SystemExplorer::Models::Items::AppThemeResourceItem{ L"Violet Red Light", L"#32B146C2" });
        resources.Append(SystemExplorer::Models::Items::AppThemeResourceItem{ L"Cool Blue Bright", L"#320099BC" });
        resources.Append(SystemExplorer::Models::Items::AppThemeResourceItem{ L"Seafoam", L"#3200B7C3" });
        resources.Append(SystemExplorer::Models::Items::AppThemeResourceItem{ L"Mint Light", L"#3200B294" });
        resources.Append(SystemExplorer::Models::Items::AppThemeResourceItem{ L"Gray", L"#327A7574" });
        resources.Append(SystemExplorer::Models::Items::AppThemeResourceItem{ L"Green", L"#32107C10" });
        resources.Append(SystemExplorer::Models::Items::AppThemeResourceItem{ L"Overcast", L"#32767676" });
        resources.Append(SystemExplorer::Models::Items::AppThemeResourceItem{ L"Storm", L"#324C4A48" });
        resources.Append(SystemExplorer::Models::Items::AppThemeResourceItem{ L"Blue Gray", L"#3269797E" });
        resources.Append(SystemExplorer::Models::Items::AppThemeResourceItem{ L"Gray Dark", L"#324A5459" });
        resources.Append(SystemExplorer::Models::Items::AppThemeResourceItem{ L"Camouflage", L"#327E735F" });

        return resources;
	}
}


