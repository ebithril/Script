#include "LuaState.h"

#include "ScriptFunctions.h"
#include "file_watcher.h"

#include <lua.hpp>

#include <assert.h>
#include <functional>

#include <numeric>
#include <Windows.h>
#include <algorithm>

namespace Script
{
	LuaState::LuaState()
	{
		myFilePath = "";
		myIsLoaded = false;

		myState = luaL_newstate();

		luaL_openlibs(myState);

		myFile.open("../scripts/exposedfunctions.txt");

		assert(myState != nullptr && "Was not able to create lua state");
	}

	LuaState::~LuaState()
	{
		myFile.close();
	}

	int LuaState::GetNumberOfArguments()
	{
		return lua_gettop(myState);
	}

	int LuaState::GetType()
	{
		return lua_type(myState, GetNumberOfArguments());
	}

	void LuaState::CallFunction(const char* aName, int aNumberOfArgs, int aNumberOfReturns)
	{
		if (myIsLoaded == false)
		{
			PrintLog("No file loaded trying to reload.");
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

			PrintLog(errorMsg.c_str());
			lua_pop(myState, 1);

			return;
		}

		CheckError(lua_pcall(myState, aNumberOfArgs, aNumberOfReturns, 0));
	}

	void LuaState::RegisterFunction(const char* aName, const lua_CFunction& aFunction, const char* aDescription)
	{
		lua_register(myState, aName, aFunction);

		myFile << "------------------------------\n" << "- Name: " << aName << "\n- Description: " << aDescription << "\n------------------------------" << std::endl;

		myExposedFunctions[aName] = aFunction;
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
			PrintLog("Error in loading file look at error message above. \n");
			myIsLoaded = false;
		}
		else if (CheckError(lua_pcall(myState, 0, LUA_MULTRET, 0)) == true)
		{
			PrintLog("Error in loading file look at error message above. \n");
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
				FindClosest(aResult);
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

			luaError += GetNiceErrorMessage(lua_tostring(myState, GetNumberOfArguments()));
			luaError += '\n';

			PrintLog(luaError.c_str());

			return true;
		}
	}

	void LuaState::FindClosest(int aError)
	{
		if (aError == LUA_ERRRUN)
		{
			std::string errorMsg = lua_tostring(myState, GetNumberOfArguments());
			
			if (errorMsg.find("global") == std::string::npos)
			{
				errorMsg = GetNiceErrorMessage(errorMsg);
				PrintLog(errorMsg.c_str());
				return;
			}

			int firstIndex = errorMsg.find_first_of('\'');
			int secondIndex = errorMsg.find_first_of('\'', firstIndex + 1);

			std::string functionBeingCalled = errorMsg.substr(firstIndex + 1, secondIndex - firstIndex - 1);

			int closest = INT_MAX;
			std::string example = "";

			auto exposedFunction = myExposedFunctions.begin();

			while (exposedFunction != myExposedFunctions.end())
			{
				int dist = levenshtein_distance(functionBeingCalled, exposedFunction->first);

				if (dist < closest)
				{
					example = exposedFunction->first;
					closest = dist;
				}
				exposedFunction++;
			}

			errorMsg = GetFileAndLine(errorMsg);
			errorMsg += " \'";
			errorMsg += functionBeingCalled + "\' doesn't exist did you mean \'" + example + "\'?";

			PrintLog(errorMsg.c_str());
		}
		else
		{
			assert(false && "No error detected but the function doesn't seem to exist, talk to programmer if this happens.");
		}
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

#undef min
	int LuaState::levenshtein_distance(const std::string &s1, const std::string &s2)
	{
		// To change the type this function manipulates and returns, change
		// the return type and the types of the two variables below.
		int s1len = s1.size();
		int s2len = s2.size();

		auto column_start = (decltype(s1len))1;

		auto column = new decltype(s1len)[s1len + 1];
		std::iota(column + column_start, column + s1len + 1, column_start);

		for (auto x = column_start; x <= s2len; x++) {
			column[0] = x;
			auto last_diagonal = x - column_start;
			for (auto y = column_start; y <= s1len; y++) {
				auto old_diagonal = column[y];
				auto possibilities = {
					column[y] + 1,
					column[y - 1] + 1,
					last_diagonal + (s1[y - 1] == s2[x - 1] ? 0 : 1)
				};
				column[y] = std::min(possibilities);
				last_diagonal = old_diagonal;
			}
		}

		auto result = column[s1len];
		delete[] column;
		return result;
	}

	void LuaState::Reload()
	{
		lua_close(myState);

		myState = luaL_newstate();

		auto exposedFunctions = myExposedFunctions.begin();

		while (exposedFunctions != myExposedFunctions.end())
		{
			lua_register(myState, exposedFunctions->first.c_str(), exposedFunctions->second);
			exposedFunctions++;
		}
	}
}