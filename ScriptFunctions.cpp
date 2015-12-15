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
			if (lua_isnil(aState, i) == 0)
			{
				string += lua_tostring(aState, i);
			}
		}

		LuaInterface::Print(string);

		return 0;
	}

	void PrintLog(const std::string& aStringToPrint)
	{
		std::cout << aStringToPrint << std::endl;

		OutputDebugStringA(aStringToPrint.c_str());
		LuaInterface::ourDebugLog << aStringToPrint << std::endl;
	}
}