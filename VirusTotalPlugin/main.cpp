#include <alpc.h>
#include <alpcplus.h>
#include <iostream>

alpc::PluginInformation g_information;
alpc::PluginClient g_client;

void initPlugin()
{
	g_information.Author = L"Test Author";
	g_information.Name = L"Syste";
	g_information.Version = 1;

	g_client = alpc::PluginClient{ g_information };
}

int main()
{
	initPlugin();

	try
	{
		uint32_t commandId = 100;

		while (true)
		{
			std::wstring text;

			std::wcout << L"[>] Enter button text (exit to close): ";

			std::getline(std::wcin, text);

			if (text == L"exit")
				break;

			if (text.empty())
				continue;


			BUTTON_INFORMATION information{};

			information.CommandId = commandId++;
			information.Location = BUTTON_LOCATION::blProcessesListMenuFlyout;

			wcsncpy_s(
				information.Text,
				ARRAYSIZE(information.Text),
				text.c_str(),
				_TRUNCATE
			);

			std::wstring tooltip = text + L" Tooltip";

			wcsncpy_s(
				information.TooltipSuggestionText,
				ARRAYSIZE(information.TooltipSuggestionText),
				tooltip.c_str(),
				_TRUNCATE
			);


			g_client.SendCommandAsync(
				ALPC_CMD_CREATE_BUTTON,
				information
			).get();


			std::wcout
				<< L"[>] Button created: "
				<< text
				<< std::endl;
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "[>] failed: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "[>] unknown error" << std::endl;
	}

	return 0;
}
//
//PLUGIN_INFORMATION g_information;
//HANDLE g_connection;
//
//
//NTSTATUS initPlugin()
//{
//    ZeroMemory(&g_information, sizeof(g_information));
//
//    wcsncpy_s(
//        g_information.Author,
//        ARRAYSIZE(g_information.Author),
//        L"Test Author",
//        _TRUNCATE
//    );
//
//    wcsncpy_s(
//        g_information.Name,
//        ARRAYSIZE(g_information.Name),
//        L"Sys",
//        _TRUNCATE
//    );
//
//    g_information.Version = 1;
//
//    return AlpcPluginStartup(
//        &g_connection,
//        &g_information
//    );
//}
//
//
//int main()
//{
//    NTSTATUS status = initPlugin();
//
//    if (!NT_SUCCESS(status))
//    {
//        printf("[X] Plugin initialization failed: 0x%X\n", status);
//        return -1;
//    }
//
//
//    printf("[>_] Plugin Test\n");
//    printf("[>_] Enter PID: ");
//
//
//    uint32_t pid;
//
//    if (scanf_s("%u", &pid) != 1)
//    {
//        printf("[X] Invalid PID\n");
//        return -1;
//    }
//
//    ALPC_RESPONSE_MESSAGE response =
//        AlpcSendCommand(
//            &g_connection,
//            ALPC_CMD_ECHO,
//            &pid,
//            sizeof(pid)
//        );
//
//
//    if (NT_SUCCESS(response.Status))
//    {
//        printf("[+] Success\n");
//    }
//    else
//    {
//        printf(
//            "[X] Failed. NTSTATUS: 0x%X\n",
//            response.Status
//        );
//    }
//    return 0;
//}