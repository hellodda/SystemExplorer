#pragma once
#include "Native.h"

__interface ISystemProcessDataSource
{
	[[nodiscard]] native::shared_process_item QueryProcesses();
};

__interface ISystemProcessManager
{

};

__interface ISystemServiceDataSource
{

};

