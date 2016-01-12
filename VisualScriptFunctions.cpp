#include "ScriptPrecompiled.h"
#include "VisualScriptFunctions.h"

#include "../include/lua.hpp"

#include "LuaInterface.h"

#include <string>
#include <algorithm>

namespace Script
{
	int ExecutePin(lua_State* aState)
	{
		LuaInterface* luaInstance = CreateLuaInterface();

		int numberOfArguments = lua_gettop(aState);

		if (numberOfArguments != 3)
		{
			LuaInterface::Print("ExecutePin only supports 3 arguments no more no less ");
			return 0;
		}

		if (lua_isnil(aState, 1) == 1)
		{
			LuaInterface::Print("First argument is nil");
			return 0;
		}

		std::string pinName = lua_tostring(aState, 1);

		std::transform(pinName.begin(), pinName.end(), pinName.begin(), std::tolower);

		if (lua_isnil(aState, 2) == 1)
		{
			LuaInterface::Print("Second argument is nil");
			return 0;
		}

		long long callerID = std::stoll((lua_tostring(aState, 2)));

		if (lua_isnil(aState, 3) == 1)
		{
			LuaInterface::Print("Third argument is nil");
			return 0;
		}

		int scriptID = int(lua_tointeger(aState, 3));

		luaInstance->ExecutePin(pinName, callerID, scriptID);

		luaInstance->Release();

		return 0;
	}

	int GetPinData(lua_State* aState)
	{
		LuaInterface* luaInstance = CreateLuaInterface();

		int numberOfArguments = lua_gettop(aState);

		if (numberOfArguments != 3)
		{
			LuaInterface::Print("ExecutePin only supports 3 arguments no more no less ");
			return 0;
		}

		if (lua_isnil(aState, 1) == 1)
		{
			LuaInterface::Print("First argument is nil");
			return 0;
		}

		std::string pinName = lua_tostring(aState, 1);

		std::transform(pinName.begin(), pinName.end(), pinName.begin(), std::tolower);

		if (lua_isnil(aState, 2) == 1)
		{
			LuaInterface::Print("Second argument is nil");
			return 0;
		}

		long long callerID = std::stoll((lua_tostring(aState, 2)));

		if (lua_isnil(aState, 3) == 1)
		{
			LuaInterface::Print("Third argument is nil");
			return 0;
		}

		int scriptID = int(lua_tointeger(aState, 3));

		luaInstance->GetPinData(pinName, callerID, scriptID, aState);


		luaInstance->Release();

		return 1;
	}
}