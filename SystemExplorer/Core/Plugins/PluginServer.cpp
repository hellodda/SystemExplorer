#include "pch.h"
#include "PluginServer.h"
#include <wil/win32_helpers.h>
#include <winrt/base.h>
#include <string_view>
#include <atomic>
#include <shared_mutex>
#include <unordered_map>

//extern winrt::SystemExplorer::Core::Plugins::PluginServer g_server{ ALPC_API_PORT_NAME };

namespace winrt::SystemExplorer::Core::Plugins
{
    //AlpcTransportServer::AlpcTransportServer(std::wstring_view portName) : portName_(portName) {}

    //AlpcTransportServer::~AlpcTransportServer()
    //{
    //    Stop();
    //}

    //NTSTATUS AlpcTransportServer::Start(RequestDelegate applicationPipeline)
    //{
    //    if (isRunning_.exchange(true)) return STATUS_SUCCESS;

    //    NTSTATUS status = CreateAlpcPort();
    //    if (!NT_SUCCESS(status))
    //    {
    //        isRunning_ = false;
    //        return status;
    //    }

    //    ListenerLoopAsync(std::move(applicationPipeline));
    //    return STATUS_SUCCESS;
    //}

    //void AlpcTransportServer::Stop()
    //{
    //    if (isRunning_.exchange(false)) {
    //        std::unique_lock portLock(portMutex_);
    //        portHandle_.reset();

    //        std::unique_lock clientsLock(clientsMutex_);
    //        activeClients_.clear();
    //    }
    //}

    //NTSTATUS AlpcTransportServer::CreateAlpcPort()
    //{
    //    UNICODE_STRING usPortName;
    //    OBJECT_ATTRIBUTES objPort;
    //    ALPC_PORT_ATTRIBUTES serverPortAttr;

    //    RtlInitUnicodeString(&usPortName, portName_.c_str());
    //    InitializeObjectAttributes(&objPort, &usPortName, 0, nullptr, nullptr);

    //    RtlSecureZeroMemory(&serverPortAttr, sizeof(serverPortAttr));
    //    serverPortAttr.MaxMessageLength = MAX_MSG_LEN;

    //    return NtAlpcCreatePort(portHandle_.put(), &objPort, &serverPortAttr);
    //}

    //winrt::fire_and_forget AlpcTransportServer::ListenerLoopAsync(RequestDelegate app)
    //{
    //    co_await winrt::resume_background();

    //    LARGE_INTEGER timeout;
    //    timeout.QuadPart = -1000000;

    //    while (isRunning_)
    //    {
    //        ALPC_REQUEST_MESSAGE reqMsg{};
    //        SIZE_T nLen = sizeof(reqMsg);
    //        HANDLE currentPort = nullptr;

    //        {
    //            std::shared_lock lock(portMutex_);
    //            if (!portHandle_) break;
    //            currentPort = portHandle_.get();
    //        }

    //        NTSTATUS status = NtAlpcSendWaitReceivePort(
    //            currentPort, 0, nullptr, nullptr,
    //            reinterpret_cast<PPORT_MESSAGE>(&reqMsg), &nLen, nullptr, &timeout
    //        );

    //        if (status == STATUS_TIMEOUT) continue;
    //        if (!NT_SUCCESS(status)) break;

    //        USHORT msgType = reqMsg.Header.u2.s2.Type & 0xFF;

    //        switch (msgType)
    //        {
    //        case LPC_CONNECTION_REQUEST:
    //            HandleConnectionRequest(reqMsg, currentPort);
    //            break;
    //        case LPC_PORT_CLOSED:
    //            HandleClientDisconnect(reqMsg);
    //            break;
    //        case LPC_REQUEST:
    //        case LPC_DATAGRAM:
    //            ProcessRequestAsync(reqMsg, currentPort, app);
    //            break;
    //        }
    //    }
    //}

    //void AlpcTransportServer::HandleConnectionRequest(ALPC_REQUEST_MESSAGE const& reqMsg, HANDLE currentPort)
    //{
    //    HANDLE hConnectedPort = nullptr;
    //    PORT_MESSAGE pmReply{};
    //    RtlSecureZeroMemory(&pmReply, sizeof(pmReply));
    //    pmReply.MessageId = reqMsg.Header.MessageId;
    //    pmReply.u1.s1.TotalLength = sizeof(PORT_MESSAGE);

    //    NTSTATUS acceptStatus = NtAlpcAcceptConnectPort(
    //        &hConnectedPort, currentPort, 0, nullptr, nullptr, nullptr,
    //        &pmReply, nullptr, TRUE
    //    );

    //    if (NT_SUCCESS(acceptStatus)) {
    //        std::unique_lock lock(clientsMutex_);
    //        uint64_t clientId = GetClientId(reqMsg.Header.ClientId);
    //        activeClients_[clientId] = wil::unique_handle(hConnectedPort);
    //    }
    //    else {
    //        LOG_NTSTATUS_MSG(acceptStatus, "NtAlpcAcceptConnectPort failed");
    //    }
    //}

    //void AlpcTransportServer::HandleClientDisconnect(ALPC_REQUEST_MESSAGE const& reqMsg)
    //{
    //    std::unique_lock lock(clientsMutex_);
    //    uint64_t clientId = GetClientId(reqMsg.Header.ClientId);
    //    activeClients_.erase(clientId);
    //}
    //winrt::fire_and_forget AlpcTransportServer::ProcessRequestAsync(ALPC_REQUEST_MESSAGE reqMsg, HANDLE currentPort, RequestDelegate app)
    //{
    //    co_await winrt::resume_background();

    //    auto context = std::make_shared<AlpcContext>();
    //    context->Request = reqMsg;
    //    context->ConnectionPort = currentPort;

    //    RtlSecureZeroMemory(&context->Response, sizeof(context->Response));
    //    context->Response.Header.MessageId = reqMsg.Header.MessageId;
    //    context->Response.Header.ClientId = reqMsg.Header.ClientId; // Оставляем это!
    //    context->Response.Header.u1.s1.DataLength = sizeof(ALPC_RESPONSE_MESSAGE) - sizeof(PORT_MESSAGE);
    //    context->Response.Header.u1.s1.TotalLength = sizeof(ALPC_RESPONSE_MESSAGE);
    //    context->Response.Response = reqMsg.Request;

    //    try
    //    {
    //        co_await app(context);
    //    }
    //    catch (...)
    //    {
    //        context->Response.Status = STATUS_UNSUCCESSFUL;
    //    }

    //    // ---> ИЩЕМ ПРАВИЛЬНЫЙ COMMUNICATION PORT КЛИЕНТА <---
    //    HANDLE replyPort = nullptr;
    //    {
    //        std::shared_lock lock(clientsMutex_);
    //        uint64_t clientId = GetClientId(reqMsg.Header.ClientId);
    //        if (auto it = activeClients_.find(clientId); it != activeClients_.end()) {
    //            replyPort = it->second.get();
    //        }
    //    }

    //    // Если клиент уже отключился и порта нет — выходим
    //    if (!replyPort) co_return;

    //    // Отправляем ответ в replyPort, а не в currentPort
    //    NTSTATUS status = NtAlpcSendWaitReceivePort(
    //        replyPort,
    //        ALPC_MSGFLG_REPLY_MESSAGE,
    //        reinterpret_cast<PPORT_MESSAGE>(&context->Response),
    //        nullptr, nullptr, nullptr, nullptr, nullptr
    //    );

    //    if (!NT_SUCCESS(status) && status != STATUS_PORT_DISCONNECTED)
    //        LOG_NTSTATUS_MSG(status, "NtAlpcSendWaitReceivePort (Reply) failed for RequestId: %lu", reqMsg.Request);
    //}

    //inline uint64_t AlpcTransportServer::GetClientId(CLIENT_ID const& cid) noexcept
    //{
    //    return (static_cast<uint64_t>(reinterpret_cast<uintptr_t>(cid.UniqueProcess)) << 32) |
    //        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(cid.UniqueThread));
    //}

    //PluginServer::PluginServer(std::wstring_view portName)
    //    : transport_(portName)
    //{
    //    ConfigurePipeline();
    //}

    //void PluginServer::Map(uint32_t requestId, RequestHandler const& handler)
    //{
    //    std::unique_lock lock(routesMutex_);
    //    routes_[requestId].push_back(handler);
    //}

    //NTSTATUS PluginServer::Start()
    //{
    //    return transport_.Start(appPipeline_);
    //}

    //void PluginServer::Stop()
    //{
    //    transport_.Stop();
    //}

    //void PluginServer::ConfigurePipeline()
    //{
    //    pipelineBuilder_.Use([](RequestDelegate next) -> RequestDelegate {
    //        return [next](std::shared_ptr<AlpcContext> ctx) -> winrt::Windows::Foundation::IAsyncAction {
    //            try
    //            {
    //                co_await next(ctx);
    //            }
    //            catch (const std::exception& e)
    //            {
    //                ctx->Response.Status = STATUS_UNSUCCESSFUL;
    //                strcpy_s(reinterpret_cast<char*>(ctx->Response.Data), DATA_BUFFER_SIZE, e.what());
    //            }
    //        };
    //    });

    //    pipelineBuilder_.Use([](RequestDelegate next) -> RequestDelegate 
    //    {
    //        return [next](std::shared_ptr<AlpcContext> ctx) -> winrt::Windows::Foundation::IAsyncAction {
    //            if (ctx->Request.Request == ALPC_CMD_PLUGIN_INIT)
    //            {
    //                alpc::AlpcRequestMessage initReq(ctx->Request);
    //                auto pluginInfoResult = initReq.ReadAs<PLUGIN_INFORMATION>();
    //                if (pluginInfoResult.has_value()) {
    //                    const auto* pInfo = &pluginInfoResult.value();
    //                    auto logMsg = wil::str_printf<std::wstring>(
    //                        L"[Plugin Server] Handshake:\n  Name: %ws\n  Version: %u\n",
    //                        pInfo->Name, pInfo->Version);
    //                    OutputDebugStringW(logMsg.c_str());
    //                }
    //            }
    //            co_await next(ctx);
    //        };
    //    });

    //    RequestDelegate endpointRouter = [this](std::shared_ptr<AlpcContext> ctx) -> winrt::Windows::Foundation::IAsyncAction {
    //        std::vector<RequestHandler> handlers; 
    //        bool hasHandlers = false;
    //        {
    //            std::shared_lock lock(routesMutex_);
    //            if (auto it = routes_.find(ctx->Request.Request); it != routes_.end()) {
    //                handlers = it->second;
    //                hasHandlers = true;
    //            }
    //        }

    //        if (!hasHandlers || handlers.empty()) {
    //            ctx->Response.Status = STATUS_INVALID_PARAMETER;
    //            strcpy_s(reinterpret_cast<char*>(ctx->Response.Data), DATA_BUFFER_SIZE, "Unknown Request ID");
    //            co_return;
    //        }

    //        alpc::AlpcRequestMessage requestWrapper(ctx->Request);
    //        alpc::AlpcResponseMessage responseWrapper;

    //        for (const auto& handler : handlers) {

    //            if (handler.MinVersion != ALPC_API_VERSION_IGNORE &&
    //                static_cast<int32_t>(ctx->Request.Version) < handler.MinVersion)
    //            {
    //                continue; 
    //            }
    //            handler.Callback(requestWrapper, responseWrapper);
    //        }

    //        ctx->Response.Status = responseWrapper.Status();
    //        auto readData = responseWrapper.ReadAs<std::array<BYTE, DATA_BUFFER_SIZE>>();
    //        if (readData.has_value())
    //        {
    //            std::memcpy(ctx->Response.Data, readData.value().data(), DATA_BUFFER_SIZE);
    //        }
    //        ctx->IsHandled = true;
    //        co_return;
    //    };

    //    appPipeline_ = pipelineBuilder_.Build(endpointRouter);
    //}
}