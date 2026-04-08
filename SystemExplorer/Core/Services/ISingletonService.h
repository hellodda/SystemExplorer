#pragma once

template<typename T>
struct ISingletonService
{
	static T& Instance()
	{
		static T service;
		return service;
	}
};