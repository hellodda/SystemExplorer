#pragma once
#include <string>
#include <wil/result.h>

namespace eil
{
	constexpr const wchar_t* failure_type_to_string(wil::FailureType type) noexcept
	{
		switch (type)
		{	
		case wil::FailureType::Exception:
			return L"Exception";
		case wil::FailureType::Return:
			return L"Return";
		case wil::FailureType::Log:
			return L"Log";
		case wil::FailureType::FailFast:
			return L"FailFast";
		default:
			return L"Unknown";
		}
	}
}