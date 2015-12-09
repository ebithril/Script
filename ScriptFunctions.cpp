#include "ScriptFunctions.h"

#include <lua.hpp>
#include <Windows.h>
#include <string>
#include <iostream>
#include "LuaInterface.h"

namespace Script
{
	int PrintLog(lua_State* aState)
	{
		int arguments = lua_gettop(aState);

		std::string string = "";

		for (int i = 1; i <= arguments; i++)
		{
			string += lua_tostring(aState, i);
		}

		return PrintLog(string.c_str());
	}

	int PrintLog(const char* aStringToPrint)
	{
		std::cout << aStringToPrint << std::endl;

		OutputDebugStringA(aStringToPrint);
		LuaInterface::ourDebugLog << aStringToPrint << std::endl;

		return 0;
	}
}