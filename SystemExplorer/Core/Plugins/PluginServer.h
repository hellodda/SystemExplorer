#pragma once
#include <alpc.h>
#include <shared_mutex>
#include <winrt/Windows.Foundation.h>
#include <memory>
#include <functional>

#define ALPC_API_VERSION_V1 1
#define ALPC_API_VERSION_V2 2
#define ALPC_API_VERSION_V3 3
#define ALPC_API_VERSION_TEST 555


namespace winrt::SystemExplorer::Core::Plugins
{
    struct AlpcContext
    {
        ALPC_REQUEST_MESSAGE Request{};
        ALPC_RESPONSE_MESSAGE Response{};
        HANDLE ConnectionPort{ nullptr };
        bool IsHandled{ false }; 
    };

    using RequestDelegate = std::function<winrt::Windows::Foundation::IAsyncAction(std::shared_ptr<AlpcContext>)>;
    using Middleware = std::function<RequestDelegate(RequestDelegate next)>;

    class AlpcPipelineBuilder
    {
    public:
        void Use(Middleware middleware)
        {
            middlewares_.push_back(std::move(middleware));
        }

        RequestDelegate Build(RequestDelegate fallbackHandler)
        {
            RequestDelegate app = std::move(fallbackHandler);

            for (auto it = middlewares_.rbegin(); it != middlewares_.rend(); ++it)
            {
                app = (*it)(app);
            }
            return app;
        }

    private:
        std::vector<Middleware> middlewares_;
    };

    class AlpcTransportServer
    {
    public:
        explicit AlpcTransportServer(std::wstring_view portName);
        ~AlpcTransportServer();

        AlpcTransportServer(const AlpcTransportServer&) = delete;
        AlpcTransportServer& operator=(const AlpcTransportServer&) = delete;

        NTSTATUS Start(RequestDelegate applicationPipeline);
        void Stop();

    private:
        NTSTATUS CreateAlpcPort();
        winrt::fire_and_forget ListenerLoopAsync(RequestDelegate app);

        void HandleConnectionRequest(ALPC_REQUEST_MESSAGE const& reqMsg, HANDLE currentPort);
        void HandleClientDisconnect(ALPC_REQUEST_MESSAGE const& reqMsg);
        winrt::fire_and_forget ProcessRequestAsync(ALPC_REQUEST_MESSAGE reqMsg, HANDLE currentPort, RequestDelegate app);

        inline uint64_t GetClientId(CLIENT_ID const& cid) noexcept;

        std::wstring portName_;
        std::atomic<bool> isRunning_{ false };

        wil::unique_handle portHandle_;
        std::shared_mutex portMutex_;

        std::unordered_map<uint64_t, wil::unique_handle> activeClients_;
        std::shared_mutex clientsMutex_;
    };

    using request_callback_handler_t = std::function<void(alpc::AlpcRequestMessage const&, alpc::AlpcResponseMessage&)>;

    struct RequestHandler
    {
        int32_t MinVersion{ ALPC_API_VERSION_IGNORE };
        request_callback_handler_t Callback;
    };

    class PluginServer
    {
    public:
        explicit PluginServer(std::wstring_view portName);

        void Map(uint32_t requestId, RequestHandler const& handler);

        NTSTATUS Start();
        void Stop();

    private:
        void ConfigurePipeline();

        AlpcTransportServer transport_;
        AlpcPipelineBuilder pipelineBuilder_;
        RequestDelegate appPipeline_;

        std::unordered_map<uint32_t, std::vector<RequestHandler>> routes_;
        std::shared_mutex routesMutex_;
    };
}

extern winrt::SystemExplorer::Core::Plugins::PluginServer g_server;