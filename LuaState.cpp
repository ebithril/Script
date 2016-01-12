#include "ScriptPrecompiled.h"
#include "LuaState.h"

#include <Windows.h>
#include "ScriptFunctions.h"
#include "file_watcher.h"

#include <lua.hpp>

#include <assert.h>
#include <functional>

#include <numeric>
#include <algorithm>

#include "LuaInterface.h"
#include "LuaArguments.h"
#include "LuaManager.h"

namespace Script
{
	LuaState::LuaState(LuaManager* aManager, int aId)
	{
		myId = aId;
		myManager = aManager;

		myFilePath = "";
		myIsLoaded = false;

		myState = luaL_newstate();

		luaL_openlibs(myState);

		assert(myState != nullptr && "Was not able to create lua state");
	}

	LuaState::~LuaState()
	{
	}

	int LuaState::GetID() const
	{
		return myId;
	}

	int LuaState::GetNumberOfArguments()
	{
		return lua_gettop(myState);
	}

	int LuaState::GetType()
	{
		return lua_type(myState, GetNumberOfArguments());
	}

	void LuaState::CallFunction(const char* aName, int aNumberOfArgs, int aNumberOfReturns, const LuaArguments& someArguments)
	{
		if (myIsLoaded == false)
		{
			LuaInterface::Print("No file loaded trying to reload.");
			UseFile(myFilePath.c_str());

			if (myIsLoaded == false)
			{
				return;
			}
		}

		int type = lua_getglobal(myState, aName);

		if (type != LUA_TFUNCTION)
		{
			std::string errorMsg = aName;
			errorMsg += " is a ";
			errorMsg += lua_typename(myState, type);
			errorMsg += " and not a function please fix this or talk to programmer.";

			LuaInterface::Print(errorMsg.c_str());
			lua_pop(myState, 1);

			return;
		}

		someArguments.PushArguments(myState, aNumberOfArgs);

		CheckError(lua_pcall(myState, aNumberOfArgs, aNumberOfReturns, 0));
	}

	void LuaState::RegisterFunction(const char* aName, const lua_CFunction& aFunction)
	{
		lua_register(myState, aName, aFunction);
	}

	void LuaState::UseFile(const char* aFilePath)
	{
		if (myIsLoaded == true)
		{
			Reload();
		}

		lua_gc(myState, LUA_GCCOLLECT, 0);

		myFilePath = aFilePath;

		if (CheckError(luaL_loadfile(myState, aFilePath)) == true)
		{
			LuaInterface::Print("Error in loading file look at error message above. \n");
			myIsLoaded = false;
		}
		else if (CheckError(lua_pcall(myState, 0, LUA_MULTRET, 0)) == true)
		{
			LuaInterface::Print("Error in loading file look at error message above. \n");
			myIsLoaded = false;
		}
		else
		{
			if (myIsLoaded == false)
			{
				std::function<void(const std::string)> function = [this](const std::string& aString) { this->UseFile(aString.c_str()); };
				CFileWatcher::GetInstance()->WatchFileChange(myFilePath, function);
			}
			
			myIsLoaded = true;
		}

		lua_gc(myState, LUA_GCCOLLECT, 0);
	}

	void LuaState::CallString(const char* aString)
	{
		if (CheckError(luaL_loadstring(myState, aString)) == true)
		{
			LuaInterface::Print("The command cannot be done");
		}
		else if (CheckError(lua_pcall(myState, 0, LUA_MULTRET, 0)) == true)
		{
			LuaInterface::Print("The command cannot be done");
		}
	}

	void LuaState::RegisterAddCallBackFunction(const std::string& aName, std::function<void(std::string, std::shared_ptr<LuaBaseScript>, long long)> aFunction)
	{
		myRegisterCallBackFunctions[aName] = aFunction;
	}

#undef max
	bool LuaState::RegisterCallback(const std::string& cppFunctionName, const std::string& luaFunctionName, std::shared_ptr<LuaState> aState, long long aGameObjectID)
	{
		for (auto it = myRegisterCallBackFunctions.begin(); it != myRegisterCallBackFunctions.end(); it++)
		{
			if (it->first == cppFunctionName)
			{
				it->second(luaFunctionName, aState, aGameObjectID);
				return true;
			}
		}

		return false;
	}

	bool LuaState::CheckError(int aResult)
	{
		if (aResult != LUA_OK)
		{
			std::string luaError = "";

			switch (aResult)
			{
			case LUA_YIELD:
				return false;
				break;
			case LUA_ERRRUN:
				myManager->FindClosest(lua_tostring(myState, GetNumberOfArguments()));
				return true;
				break;
			case LUA_ERRSYNTAX:
				luaError = "Syntax error: ";
				break;
			case LUA_ERRMEM:
				luaError = "Memory error: ";
				break;
			case LUA_ERRGCMM:
				luaError = "Global memory error: ";
				break;
			case LUA_ERRERR:
				luaError = "Error: ";
				break;
			default:
				break;
			}
			//luaError += lua_tostring(myState, -1);
			luaError += lua_tostring(myState, 1);
			luaError += GetNiceErrorMessage(lua_tostring(myState, GetNumberOfArguments()));
			luaError += '\n';

			LuaInterface::Print(luaError.c_str());

			return true;
		}
		return false;
	}

	std::string LuaState::GetNiceErrorMessage(const std::string& aLuaError)
	{
		int firstIndex = 0;
		int secondIndex = aLuaError.find_first_of(':');
		std::string fileName = aLuaError.substr(firstIndex, secondIndex);
		firstIndex = secondIndex;
		secondIndex = aLuaError.find_first_of(':', firstIndex + 1);
		std::string lineNumber = aLuaError.substr(firstIndex + 1, secondIndex - firstIndex - 1);
		firstIndex = secondIndex;
		secondIndex = aLuaError.length();
		std::string errorMsg = aLuaError.substr(firstIndex + 1, secondIndex - firstIndex - 1);

		std::string output = "File: ";
		output += fileName + " Line: " + lineNumber + " Error Message: " + errorMsg;

		return output;
	}

	std::string LuaState::GetFileAndLine(const std::string& aLuaError)
	{
		int firstIndex = 0;
		int secondIndex = aLuaError.find_first_of(':');
		std::string fileName = aLuaError.substr(firstIndex, secondIndex);
		firstIndex = secondIndex;
		secondIndex = aLuaError.find_first_of(':', firstIndex + 1);
		std::string lineNumber = aLuaError.substr(firstIndex + 1, secondIndex - firstIndex - 1);

		std::string output = "File: ";
		output += fileName + " Line: " + lineNumber;

		return output;
	}

	void LuaState::Reload()
	{
		lua_close(myState);

		myState = luaL_newstate();

		myManager->ReloadState(this);
	}
}