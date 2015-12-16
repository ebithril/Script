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

	int RegisterCallback(lua_State* aState)
	{
		int numberOfArguments = lua_gettop(aState);

		if (numberOfArguments != 4)
		{
			LuaInterface::Print("Registercallback only supports 4 arguments no more no less ");
			return 0;
		}

		if (lua_isnil(aState, 1) == 1)
		{
			LuaInterface::Print("First argument is nil");
			return 0;
		}

		std::string cppFunctionName = lua_tostring(aState, 1);
	
		if (lua_isnil(aState, 2) == 1)
		{
			LuaInterface::Print("First argument is nil");
			return 0;
		}

		std::string luaFunctionName = lua_tostring(aState, 2);

		if (lua_isnil(aState, 3) == 1)
		{
			LuaInterface::Print("First argument is nil");
			return 0;
		}

		int scriptId = int(lua_tonumber(aState, 3));

		int gameObjectID = int(lua_tonumber(aState, 4));

		LuaInterface* instance =  CreateLuaInterface();

		instance->RegisterCallback(cppFunctionName, luaFunctionName, scriptId, gameObjectID);

		instance->Release();

		return 0;
	}
}