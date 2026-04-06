#include "pch.h"
#include "Win32Helper.h"

struct ReadOperationContext {
	wil::unique_hfile fileHandle;
	wil::unique_event ioEvent;
	std::vector<char> buffer;
	OVERLAPPED overlapped = {};
	concurrency::task_completion_event<std::wstring> tce;
	HANDLE waitHandle = nullptr;

	ReadOperationContext() {
		ioEvent.create();
		overlapped.hEvent = ioEvent.get();
	}
};

namespace winrt::SystemExplorer::Helpers
{
    concurrency::task<std::wstring> Win32Helper::ReadStringFromFileAsync(std::wstring const& filePath)
    {
        wil::unique_hfile file{ CreateFileW(
            filePath.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
            NULL
        ) };

        if (!file.is_valid()) {
            return concurrency::create_task([] { return std::wstring(L""); });
        }

        LARGE_INTEGER fileSize;
        if (!GetFileSizeEx(file.get(), &fileSize)) {
            return concurrency::create_task([] { return std::wstring(L""); });
        }

        auto ctx = new ReadOperationContext();
        ctx->fileHandle = std::move(file);
        ctx->buffer.resize(static_cast<size_t>(fileSize.QuadPart));
        auto tce = ctx->tce;

        BOOL readResult = ReadFile(
            ctx->fileHandle.get(),
            ctx->buffer.data(),
            static_cast<DWORD>(ctx->buffer.size()),
            NULL,
            &ctx->overlapped
        );

        DWORD lastError = GetLastError();

        if (!readResult && lastError == ERROR_IO_PENDING)
        {
            RegisterWaitForSingleObject(
                &ctx->waitHandle,
                ctx->ioEvent.get(),
                [](PVOID lpParameter, BOOLEAN /*TimerOrWaitFired*/) {

                    auto context = static_cast<ReadOperationContext*>(lpParameter);

                    DWORD bytesTransferred = 0;
                    if (GetOverlappedResult(context->fileHandle.get(), &context->overlapped, &bytesTransferred, FALSE)) {
                        int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, context->buffer.data(), (int)bytesTransferred, nullptr, 0);
                        std::wstring wResult(sizeNeeded, 0);
                        MultiByteToWideChar(CP_UTF8, 0, context->buffer.data(), (int)bytesTransferred, wResult.data(), sizeNeeded);

                        context->tce.set(wResult);
                    }
                    else
                        context->tce.set(L"");

                    if (context->waitHandle) 
                        THROW_IF_WIN32_BOOL_FALSE(UnregisterWait(context->waitHandle));
                    
                    delete context;
                },
                ctx, 
                INFINITE,
                WT_EXECUTEONLYONCE
            );

            return concurrency::create_task(tce);
        }

        if (readResult || lastError == ERROR_SUCCESS) {
            DWORD bytesTransferred = 0;
            GetOverlappedResult(ctx->fileHandle.get(), &ctx->overlapped, &bytesTransferred, FALSE);

            int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, ctx->buffer.data(), (int)bytesTransferred, nullptr, 0);
            std::wstring wResult(sizeNeeded, 0);
            MultiByteToWideChar(CP_UTF8, 0, ctx->buffer.data(), (int)bytesTransferred, wResult.data(), sizeNeeded);
            delete ctx;

            return concurrency::create_task([wResult]() { return wResult; });
        }
        delete ctx;
        return concurrency::create_task([] { return std::wstring(L""); });
    }

	bool Win32Helper::WriteStringToFile(std::wstring const& filePath, std::wstring const& content, DWORD flags)
	{
		return false;
	}
    std::wstring Win32Helper::GetErrorMessage(DWORD code)
    {
        wil::unique_hmodule ntdll(GetModuleHandleW(L"ntdll.dll"));
        LPWSTR messageBuffer{ nullptr };

        auto size = FormatMessageW(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
            (ntdll ? FORMAT_MESSAGE_FROM_HMODULE : 0) | FORMAT_MESSAGE_IGNORE_INSERTS,
            ntdll.get(),
            code,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPWSTR)&messageBuffer,
        0, NULL);

        if (size > 0 && messageBuffer) {
            wil::unique_process_heap_ptr<wchar_t> smartBuffer(messageBuffer);
            return std::wstring(smartBuffer.get());
        }
        return L"Unknown Error";
    }
}

