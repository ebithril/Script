#include "LuaStateHelper.h"
#include "../include/lua.hpp"
#include "../Script/LuaInterface.h"
#include <assert.h>

namespace Script
{
	LuaStateHelper::LuaStateHelper(lua_State* aLuaState, const std::string& aFunctionName)
	{
		myLuaState = aLuaState;
		myFunctionName = aFunctionName;
		if (myLuaState == nullptr)
		{
			assert(false && "Cannot init LuaStateHelper with nullptr");
		}
	}

	LuaStateHelper::~LuaStateHelper()
	{
	}

	bool LuaStateHelper::GetDataAt(std::string& aString, const int anIndex, const std::string& aCustomErrorMessage)
	{
		if (lua_isstring(myLuaState, anIndex) == LUA_FALSE)
		{
			std::string errorMessage = "String argument " + std::to_string(anIndex) + " in " + myFunctionName + " is invalid.\n" + aCustomErrorMessage;
			lua_pushstring(myLuaState, errorMessage.c_str());
			return false;
		}

		aString = lua_tostring(myLuaState, anIndex);

		return true;
	}

	bool LuaStateHelper::GetDataAt(float& aFloat, const int anIndex, const std::string& aCustomErrorMessage)
	{
		if (lua_isnumber(myLuaState, anIndex) == LUA_FALSE)
		{
			std::string errorMessage = "Number argument " + std::to_string(anIndex) + " in " + myFunctionName + " is invalid.\n" + aCustomErrorMessage;
			lua_pushstring(myLuaState, errorMessage.c_str());
			return false;
		}

		aFloat = static_cast<float>(lua_tonumber(myLuaState, anIndex));

		return true;
	}

	bool LuaStateHelper::GetDataAt(int& anInt, const int anIndex, const std::string& aCustomErrorMessage)
	{
		if (lua_isnumber(myLuaState, anIndex) == LUA_FALSE)
		{
			std::string errorMessage = "Number argument " + std::to_string(anIndex) + " in " + myFunctionName + " is invalid.\n" + aCustomErrorMessage;
			lua_pushstring(myLuaState, errorMessage.c_str());
			return false;
		}

		anInt = static_cast<int>(lua_tonumber(myLuaState, anIndex));

		return true;
	}

	bool LuaStateHelper::GetDataAt(bool& aBool, const int anIndex, const std::string& aCustomErrorMessage)
	{
		if (lua_isboolean(myLuaState, anIndex) == LUA_FALSE)
		{
			std::string errorMessage = "Boolean argument " + std::to_string(anIndex) + " in " + myFunctionName + " is invalid.\n" + aCustomErrorMessage;
			lua_pushstring(myLuaState, errorMessage.c_str());
			return false;
		}

		int luaBool = lua_toboolean(myLuaState, anIndex);

		if (luaBool == LUA_FALSE)
		{
			aBool = false;
		}
		else
		{
			aBool = true;
		}

		return true;
	}

	void LuaStateHelper::PushData(const std::string& aString)
	{
		lua_pushstring(myLuaState, aString.c_str());
	}

	void LuaStateHelper::PushData(const float& afloat)
	{
		lua_pushnumber(myLuaState, afloat);
	}

	void LuaStateHelper::PushData(const int& anInt)
	{
		lua_pushnumber(myLuaState, anInt);

	}

	void LuaStateHelper::PushData(const bool& aBool)
	{
		lua_pushboolean(myLuaState, aBool);
	}

	int LuaStateHelper::GetNumberOfArguments()
	{
		return lua_gettop(myLuaState);
	}

	bool LuaStateHelper::CheckAmountOfArguments(const int anAmount, std::string aCustomErrorMessage)
	{
		if (GetNumberOfArguments() != anAmount)
		{
			std::string errorMessage = "Incorrect amount of arguments in " + myFunctionName + "\n" + aCustomErrorMessage;
			lua_pushstring(myLuaState, errorMessage.c_str());
			return false;
		}

		return true;
	}

	int LuaStateHelper::ReturnError()
	{
		return lua_error(myLuaState);
	}

}

