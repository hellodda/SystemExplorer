#include "pch.h"
#include "NativeProcess.h"
#include "../ShellHelper.h"

#include <Core/System/Native.h>

namespace winrt::SystemExplorer::Helpers::Win32::Native
{
	bool NativeProcess::IsEfficiencyModeEnabled(uint64_t process)
	{
		native::unique_nt_handle handle{ nullptr };
		BOOLEAN isEnabled{ FALSE };

		THROW_IF_FAILED(SeOpenProcess(
			&handle,
			PROCESS_QUERY_LIMITED_INFORMATION,
			(HANDLE)process
		));

		THROW_IF_FAILED(SeIsProcessEfficiencyModeEnabled(
			handle.get(),
			&isEnabled
		));
		return isEnabled == TRUE;
	}


	std::wstring NativeProcess::GetProcessImageName(uint64_t process)
	{
		native::unique_nt_handle handle{ nullptr };
		PWSTR fileName{ nullptr };

		THROW_IF_FAILED(SeOpenProcess(
			&handle,
			PROCESS_QUERY_LIMITED_INFORMATION,
			(HANDLE)process
		));

		THROW_IF_FAILED(SeGetProcessImageFileNameWin32(
			handle.get(),
			&fileName
		));
		return fileName;
	}

	wil::unique_hicon NativeProcess::GetProcessIcon(std::wstring const& fileName)
	{
		wil::unique_hicon icon{ nullptr };

		if (!SeExtractIcon(fileName.c_str(), &icon, NULL))
		{
			icon = ShellHelper::GetIconBySIID(SIID_APPLICATION);

		}
		return icon;
	}

	void NativeProcess::TerminateProcess(uint32_t pid)
	{
		native::unique_nt_handle handle{ nullptr };

		THROW_IF_FAILED(SeOpenProcess(
			&handle,
			PROCESS_QUERY_LIMITED_INFORMATION,
			(HANDLE)pid
		));

		THROW_IF_FAILED(NtTerminateProcess(
			handle.get(),
			EXIT_SUCCESS
		));
	}
}