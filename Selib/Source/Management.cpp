#include "pch.h"
#include "Management.h"

namespace winrt::SystemExplorer::Core::Management
{
	std::vector<std::pair<winrt::hstring, _variant_t>> ManagementClassObject::GetProperties() const
	{
		std::vector<std::pair<winrt::hstring, _variant_t>> properties;

		if (!object_)
			return properties;

		HRESULT hr = object_->BeginEnumeration(WBEM_FLAG_NONSYSTEM_ONLY);
		if (FAILED(hr)) return properties;

		auto guard = wil::scope_exit([this] { object_->EndEnumeration(); });

		_bstr_t name;
		_variant_t variant;

		while (true)
		{
			hr = object_->Next(0, name.GetAddress(), &variant, nullptr, nullptr);

			if (hr == WBEM_S_NO_ERROR)
				properties.emplace_back(name, variant);
			else
				break;
		}
		return properties;
	}

	std::pair<winrt::hstring, _variant_t> ManagementClassObject::GetProperty(_In_ winrt::hstring const& name) const
	{
		_variant_t variant;

		if (object_)
			object_->Get(name.c_str(), 0, &variant, nullptr, nullptr);
		return { name, variant };
	}

	concurrency::task<std::vector<ManagementClassObject>> ManagementConnection::ExecuteQueryAsync(winrt::hstring const& query)
	{
		if (!services_) [[unlikely]]
			THROW_HR(E_POINTER);

		auto sink = winrt::make_self<QuerySink>();

		THROW_IF_FAILED(services_->ExecQueryAsync(
			_bstr_t(L"WQL"),
			_bstr_t(query.c_str()),
			NULL,
			NULL,
			sink.get()
		));
		co_return co_await sink->GetResultsAsync();
	}
	void ManagementConnection::initialize()
	{
		winrt::com_ptr<IWbemLocator> locator;

		THROW_IF_FAILED(CoCreateInstance(
			CLSID_WbemLocator,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(locator.put())
		));

		THROW_IF_FAILED(locator->ConnectServer(
			_bstr_t(connectionNamespace_.c_str()),
			_bstr_t(user_.c_str()),
			_bstr_t(password_.c_str()),
			0,
			NULL,
			0,
			0,
			services_.put()
		));

		THROW_IF_FAILED(CoSetProxyBlanket(
			services_.get(),
			RPC_C_AUTHN_WINNT,
			RPC_C_AUTHZ_NONE,
			NULL,
			RPC_C_AUTHN_LEVEL_CALL,
			RPC_C_IMP_LEVEL_IMPERSONATE,
			NULL,
			EOAC_NONE
		));
	}


	HRESULT QuerySink::Indicate(
		_In_ LONG lObjectCount,
		_In_reads_(lObjectCount) IWbemClassObject** apObjArray
	)
	{
		if (!apObjArray) [[unlikely]]
			return E_POINTER;

		for (LONG i = 0; i < lObjectCount; i++)
		{
			results_.emplace_back(ManagementClassObject(apObjArray[i]));
		}
		return WBEM_S_NO_ERROR;
	}
	HRESULT QuerySink::SetStatus(
		_In_ LONG lFlags,
		_In_ HRESULT hResult,
		_In_opt_ BSTR strParam,
		_In_opt_ IWbemClassObject* pObjParam
	)
	{
		queryCompletedEvent_.set(std::move(results_));
		return S_OK;
	}
	concurrency::task<std::vector<ManagementClassObject>> QuerySink::GetResultsAsync() const
	{
		return concurrency::task<std::vector<ManagementClassObject>>(queryCompletedEvent_);
	}
}
