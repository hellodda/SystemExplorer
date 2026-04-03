#pragma once

namespace winrt::SystemExplorer::Core::Management
{
	struct ManagementClassObject
	{
		ManagementClassObject(std::nullptr_t) : object_(nullptr) {}
		ManagementClassObject(_In_ IWbemClassObject* object) { object_.copy_from(object); }

		[[nodiscard]] std::vector<std::pair<winrt::hstring, _variant_t>> GetProperties() const;
		[[nodiscard]] std::pair<winrt::hstring, _variant_t> GetProperty(_In_ winrt::hstring const& name) const;

	private:
		winrt::com_ptr<IWbemClassObject> object_{ nullptr };
	};

	struct ManagementConnection
	{
		ManagementConnection(_In_ winrt::hstring const& connectionNamespace)
			: connectionNamespace_(connectionNamespace) { initialize(); }

		ManagementConnection(
			_In_ winrt::hstring const& connectionNamespace,
			_In_ winrt::hstring const& user,
			_In_ winrt::hstring const& password
		)
			: connectionNamespace_(connectionNamespace),
			user_(user), password_(password) { initialize(); }

		[[nodiscard]] concurrency::task<std::vector<ManagementClassObject>> ExecuteQueryAsync(_In_ winrt::hstring const& query);

	private:
		void initialize();
	private:
		winrt::com_ptr<IWbemServices> services_{ nullptr };

		winrt::hstring connectionNamespace_{};
		winrt::hstring user_{};
		winrt::hstring password_{};
	};

	struct QuerySink : winrt::implements<QuerySink, IWbemObjectSink>
	{
		[[nodiscard]] HRESULT STDMETHODCALLTYPE Indicate(
			_In_ LONG lObjectCount,
			_In_reads_(lObjectCount) IWbemClassObject** apObjArray
		) override;

		[[nodiscard]] HRESULT STDMETHODCALLTYPE SetStatus(
			_In_ LONG lFlags,
			_In_ HRESULT hResult,
			_In_opt_ BSTR strParam,
			_In_opt_ IWbemClassObject* pObjParam
		) override;

		concurrency::task<std::vector<ManagementClassObject>> GetResultsAsync() const;

	private:
		concurrency::task_completion_event<std::vector<ManagementClassObject>> queryCompletedEvent_;
		std::vector<ManagementClassObject> results_;
	};
}