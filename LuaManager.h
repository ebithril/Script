#pragma once

#include <vector>
#include "FunctionInformation.h"
#include <unordered_map>
#include <fstream>
#include <memory>

struct lua_State;

class LuaArguments;

namespace std
{
	template <typename Type>
	class function;
}
typedef int(*lua_CFunction) (lua_State *L);

struct LuaFunction
{
	FunctionInformation myInformation;
	lua_CFunction myFunction;
};

namespace Script
{
	class LuaInterface;
	class LuaState;
	class LuaBaseScript;

	class LuaManager
	{
	friend LuaInterface;

	public:
		void Init(const std::function<void()>& aFunction);

		void Update();
		void RegisterFunction(const char* aName, const lua_CFunction& aFunction, const char* aDescription);

		void FindClosest(std::string aErrorMessage);
		void ReloadState(LuaState* aState);

		std::vector<FunctionInformation> GetFunctionInfo();

		void RegisterAddCallBackFunction(const std::string& aName, std::function<void(std::string, std::shared_ptr<LuaBaseScript>, long long)> aFunction);
		void RegisterCallback(const std::string& cppFunctionName, const std::string& luaFunctionName, int scriptId, long long aGameObjectID);
	private:
		std::shared_ptr<LuaState> CreateLuaState();
		std::string GetFileAndLine(const std::string& aLuaError);
		int levenshtein_distance(const std::string &s1, const std::string &s2);

		LuaManager();
		~LuaManager();

		std::unordered_map<std::string, std::function<void(std::string, std::shared_ptr<LuaBaseScript>, long long)>> myRegisterCallBackFunctions;

		std::unordered_map<std::string, LuaFunction> myExposedFunctions;
		std::ofstream myExposedFunctionsFile;
		std::vector<std::shared_ptr<LuaState>> myStates;
		int myNextStateId;
	};
}