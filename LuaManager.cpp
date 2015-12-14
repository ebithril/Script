#include "LuaManager.h"

#include "LuaState.h"
#include "file_watcher.h"
#include "ScriptFunctions.h"
#include "LuaArguments.h"


#include <lua.hpp>
#include <functional>
#include <assert.h>

#include <windows.h>
#include <string.h>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <cmath>

#include "LuaInterface.h"

namespace Script
{
	void LuaManager::Init(const std::function<void()>& aFunction)
	{
		lua_CFunction logFunction = (lua_CFunction)&PrintLog;

		myExposedFunctionsFile.open("../scripts/exposedfunctions.txt");

		RegisterFunction("Print", logFunction, "Print a message to the log");

		if (aFunction != nullptr)
		{
			aFunction();
		}
	}

	void LuaManager::Update()
	{
		CFileWatcher::GetInstance()->FlushChanges();
	}

	void LuaManager::RegisterFunction(const char* aName, const lua_CFunction& aFunction, const char* aDescription)
	{
		for (std::shared_ptr<LuaState>& state : myStates)
		{
			state->RegisterFunction(aName, aFunction);
		}

		myExposedFunctionsFile << "------------------------------\n" << "- Name: " << aName << "\n- Description: " << aDescription << "\n------------------------------" << std::endl;

		LuaFunction func;
		func.myFunction = aFunction;

		FunctionInformation information;
		information.myName = aName;
		information.myDescription = aDescription;

		func.myInformation = information;

		myExposedFunctions[aName] = func;
	}

	std::string LuaManager::GetFileAndLine(const std::string& aLuaError)
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
	int LuaManager::levenshtein_distance(const std::string &s1, const std::string &s2)
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

	LuaManager::LuaManager()
	{
	}

	LuaManager::~LuaManager()
	{
	}

	std::shared_ptr<LuaState> LuaManager::CreateLuaState()
	{
		std::shared_ptr<LuaState> newState(new LuaState(this));

		myStates.push_back(newState);

		for (auto it = myExposedFunctions.begin(); it != myExposedFunctions.end(); it++)
		{
			newState->RegisterFunction(it->first.c_str(), it->second.myFunction);
		}

		return newState;
	}

	void LuaManager::FindClosest(std::string aErrorMessage)
	{
		int firstIndex = aErrorMessage.find_first_of('\'');
		int secondIndex = aErrorMessage.find_first_of('\'', firstIndex + 1);

		std::string functionBeingCalled = aErrorMessage.substr(firstIndex + 1, secondIndex - firstIndex - 1);

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

		aErrorMessage = GetFileAndLine(aErrorMessage);
		aErrorMessage += " \'";
		aErrorMessage += functionBeingCalled + "\' doesn't exist did you mean \'" + example + "\'?";

		LuaInterface::Print(aErrorMessage.c_str());
	}

	void LuaManager::ReloadState(LuaState* aState)
	{
		auto exposedFunctions = myExposedFunctions.begin();

		while (exposedFunctions != myExposedFunctions.end())
		{
			aState->RegisterFunction(exposedFunctions->first.c_str(), exposedFunctions->second.myFunction);
			exposedFunctions++;
		}
	}

	std::vector<FunctionInformation> LuaManager::GetFunctionInfo()
	{
		std::vector<FunctionInformation> information;

		for (auto it = myExposedFunctions.begin(); it != myExposedFunctions.end(); it++)
		{
			information.push_back(it->second.myInformation);
		}

		return information;
	}
}