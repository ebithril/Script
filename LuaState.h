#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>

#include "FunctionInformation.h"

struct lua_State;
typedef int(*lua_CFunction) (lua_State *L);

class LuaArguments;

namespace Script
{
	class LuaManager;
	class LuaState
	{
	public:
		LuaState(LuaManager* aManager);
		~LuaState();

		int GetNumberOfArguments();
		int GetType();
		void CallFunction(const char* aName, int aNumberOfArgs, int aNumberOfReturns, LuaArguments& someArguments); /* 5 max number of arguments */
		void RegisterFunction(const char* aName, const lua_CFunction& aFunction);
		void UseFile(const char* aFilePath);
		void CallString(const char* aString);
	private:
		bool CheckError(int aResult);
		std::string GetNiceErrorMessage(const std::string& aLuaError);
		std::string GetFileAndLine(const std::string& aLuaError);

		void Reload();

		std::string myFilePath;
		lua_State* myState;
		LuaManager* myManager;
		bool myIsLoaded;
	};
}