#include <iostream>

#include <windows.h>
#include <emsmsg.h>

#define SYSX_DEFAULT_TIMEOUT 1000 // timeout override

int main()
{
	EMSM_REGISTER_MODULE Module;
	HANDLE ConnectionHandle;

	if (SUCCEEDED(EmsConnectToHost(&ConnectionHandle, &Module)))
	{
		if (SUCCEEDED(Module.Status))
		{
			std::cout << "Success: Module registered successfuly" << std::endl;
		}
	}
	else
	{
		std::cout << "Error: Connection failed" << std::endl;
	}
	return 0;
}