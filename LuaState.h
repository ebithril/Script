#pragma once

#include <string>
#include <map>
#include <fstream>

struct lua_State;
typedef int(*lua_CFunction) (lua_State *L);

namespace Script
{
	class LuaState
	{
	public:
		LuaState();
		~LuaState();

		int GetNumberOfArguments();
		int GetType();
		void CallFunction(const char* aName, int aNumberOfArgs, int aNumberOfReturns);
		void RegisterFunction(const char* aName, const lua_CFunction& aFunction, const char* aDescription);
		void UseFile(const char* aFilePath);
		void CallString(const char* aString);
	private:
		bool CheckError(int aResult);
		void FindClosest(int aError);
		std::string GetNiceErrorMessage(const std::string& aLuaError);
		std::string GetFileAndLine(const std::string& aLuaError);
		int levenshtein_distance(const std::string &s1, const std::string &s2);

		void Reload();

		std::string myFilePath;
		std::map<std::string, lua_CFunction> myExposedFunctions;
		std::ofstream myFile;
		lua_State* myState;
		bool myIsLoaded;
	};
}