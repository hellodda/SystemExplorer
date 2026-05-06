#include "pch.h"
#include "AppThemeResourceFactory.h"

#include <Helpers/StringsHelper.h>

using namespace winrt::SystemExplorer::Helpers;

namespace winrt::SystemExplorer::Core::Data::Factories
{
	IObservableVector<SystemExplorer::Core::Data::Items::AppThemeResourceItem> AppThemeResourceFactory::AppThemeResources()
	{
        auto resources = single_threaded_observable_vector<SystemExplorer::Core::Data::Items::AppThemeResourceItem>();

        resources.Append(SystemExplorer::Core::Data::Items::AppThemeResourceItem{ StringsHelper::Default(), L"#00000000"});
        resources.Append(SystemExplorer::Core::Data::Items::AppThemeResourceItem{ StringsHelper::YellowGold(), L"#32FFB900"});
        resources.Append(SystemExplorer::Core::Data::Items::AppThemeResourceItem{ StringsHelper::OrangeBright(), L"#32F7630C"});
        resources.Append(SystemExplorer::Core::Data::Items::AppThemeResourceItem{ StringsHelper::BrickRed(), L"#32D13438"});
        resources.Append(SystemExplorer::Core::Data::Items::AppThemeResourceItem{ StringsHelper::ModRed(), L"#32FF4343"});
        resources.Append(SystemExplorer::Core::Data::Items::AppThemeResourceItem{ StringsHelper::Red(), L"#32EA005E"});
        resources.Append(SystemExplorer::Core::Data::Items::AppThemeResourceItem{ StringsHelper::RoseBright(), L"#32EA005E"});
        resources.Append(SystemExplorer::Core::Data::Items::AppThemeResourceItem{ StringsHelper::Blue(), L"#320078D7"});
        resources.Append(SystemExplorer::Core::Data::Items::AppThemeResourceItem{ StringsHelper::IrisPastel(), L"#328764B8"});
        resources.Append(SystemExplorer::Core::Data::Items::AppThemeResourceItem{ StringsHelper::VioletRedLight(), L"#32B146C2"});
        resources.Append(SystemExplorer::Core::Data::Items::AppThemeResourceItem{ StringsHelper::CoolBlueBright(), L"#320099BC"});
        resources.Append(SystemExplorer::Core::Data::Items::AppThemeResourceItem{ StringsHelper::Seafoam(), L"#3200B7C3"});
        resources.Append(SystemExplorer::Core::Data::Items::AppThemeResourceItem{ StringsHelper::MintLight(), L"#3200B294"});
        resources.Append(SystemExplorer::Core::Data::Items::AppThemeResourceItem{ StringsHelper::Gray(), L"#327A7574"});
        resources.Append(SystemExplorer::Core::Data::Items::AppThemeResourceItem{ StringsHelper::Green(), L"#32107C10"});
        resources.Append(SystemExplorer::Core::Data::Items::AppThemeResourceItem{ StringsHelper::Overcast(), L"#32767676"});
        resources.Append(SystemExplorer::Core::Data::Items::AppThemeResourceItem{ StringsHelper::Storm(), L"#324C4A48"});
        resources.Append(SystemExplorer::Core::Data::Items::AppThemeResourceItem{ StringsHelper::BlueGray(), L"#3269797E"});
        resources.Append(SystemExplorer::Core::Data::Items::AppThemeResourceItem{ StringsHelper::GrayDark(), L"#324A5459"});
        resources.Append(SystemExplorer::Core::Data::Items::AppThemeResourceItem{ StringsHelper::Camouflage(), L"#327E735F"});

        return resources;
	}
}


