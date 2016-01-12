#include "ScriptPrecompiled.h"
#include "ScriptFunctions.h"

#include "LuaInterface.h"
#include <lua.hpp>
#include <Windows.h>
#include <string>
#include <iostream>
#include <filesystem>

namespace Script
{
	int PrintLog(lua_State* aState)
	{
		int arguments = lua_gettop(aState);

		std::string string = "";

		for (int i = 1; i <= arguments; i++)
		{
			int argType = lua_type(aState, i);

			switch (argType)
			{
			case LUA_TNIL:
				string += "nil";
				break;
			case LUA_TNUMBER:
				string += std::to_string(static_cast<double>(lua_tonumber(aState, i)));
				break;
			case LUA_TSTRING:
				string += std::string(lua_tostring(aState, i));
				break;
			case LUA_TBOOLEAN:
				if (lua_toboolean(aState, i) == 0)
					string += std::string("false");
				else
					string += std::string("true");
				break;
			default:
				string += "INVALID";
				break;
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

		long long gameObjectID = std::stoll(lua_tostring(aState, 4));

		LuaInterface* instance =  CreateLuaInterface();

		instance->RegisterCallback(cppFunctionName, luaFunctionName, scriptId, gameObjectID);

		instance->Release();

		return 0;
	}

	std::vector<std::string> GetFilesWithExtension(const std::string& aDirectory, const std::string& anExtension)
	{
		std::vector<std::string> myfiles(4);

		for (std::tr2::sys::recursive_directory_iterator i(aDirectory.c_str()), end; i != end; ++i)
		{
			if (!is_directory(i->path()))
			{
				if (strstr(i->path().filename().c_str(), anExtension.c_str()))
				{
					myfiles.push_back(i->path().relative_path());
				}
			}
		}


		return myfiles;
	}
}