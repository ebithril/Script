#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <vector>
#include <functional>

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
		LuaState(LuaManager* aManager, int aId);
		~LuaState();

		int GetID() const;

		void CallFunction(const char* aName, int aNumberOfArgs, int aNumberOfReturns, LuaArguments& someArguments); /* 5 max number of arguments */
		void RegisterFunction(const char* aName, const lua_CFunction& aFunction);
		void UseFile(const char* aFilePath);
		void CallString(const char* aString);
		void RegisterAddCallBackFunction(const std::string& aName, std::function<void(std::string, std::shared_ptr<LuaState>, int)> aFunction);
		bool RegisterCallback(const std::string& cppFunctionName, const std::string& luaFunctionName, std::shared_ptr<LuaState> aState, int aGameObjectID);
	private:
		int GetNumberOfArguments();
		int GetType();

		bool CheckError(int aResult);
		
		std::string GetNiceErrorMessage(const std::string& aLuaError);
		std::string GetFileAndLine(const std::string& aLuaError);

		void Reload();

		std::unordered_map<std::string, std::function<void(std::string, std::shared_ptr<LuaState>, int)>> myRegisterCallBackFunctions;

		std::string myFilePath;
		lua_State* myState;
		LuaManager* myManager;
		bool myIsLoaded;
		int myId;
	};
}