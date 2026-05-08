#pragma once

#include "Controls/DataRow.g.h"

namespace winrt::SystemExplorer::Xaml::Controls::implementation
{
	using namespace winrt::Microsoft::UI::Xaml;
	using namespace winrt::Microsoft::UI::Xaml::Controls;
	using namespace winrt::Windows::Foundation;

	struct DataRow : DataRowT<DataRow>
	{
		DataRow();

		Size MeasureOverride(Size availableSize);

		Size ArrangeOverride(Size finalSize);

	private:
		Panel InitializeParentHeaderConnection();

		void DataRow_Unloaded(winrt::Windows::Foundation::IInspectable const& sender, RoutedEventArgs const& e);

		Panel _parentPanel{ nullptr };
		winrt::SystemExplorer::Xaml::Controls::DataTable _parentTable;

		bool _isTreeView{ false };
		double _treePadding{ 0.0 };
	};
}

namespace winrt::SystemExplorer::Xaml::Controls::factory_implementation
{
    struct DataRow : DataRowT<DataRow, implementation::DataRow>
    {
    };
}
