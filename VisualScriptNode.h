#pragma once

#include <vector>
#include <string>
#include "VisualScriptPin.h"
#include "LuaState.h"

struct lua_State;
typedef int(*lua_CFunction) (lua_State *L);

namespace Script
{
	class VisualScriptNode
	{
	public:
		VisualScriptNode();
		~VisualScriptNode();

		void Run(const char* aCallerID, int aScriptID);
		void Load(const std::string& aScriptFile);

		std::vector<VisualScriptPin>& GetPins();
		void CallFunction(const char* aName, const char* aID, int aScriptID);
		const std::string& GetName() const;
		bool IsStartNode();

		void RegisterFunction(const char* aName, lua_CFunction aFunction);
	private:
		void CallFunction(const char* aName, int aNumberOfArguments, int aNumberOfReturns, const LuaArguments& someArguments);
		bool CheckError(int aResult);

		std::string GetNiceErrorMessage(const std::string& aLuaError);
		std::string GetName(const std::string& aLine);
		eVisualScriptVariableTypes GetVariable(const std::string& aLine);
		std::vector<VisualScriptPin> myPins;

		std::string myName;
		lua_State* myState;
		bool myIsInitNode;
	};
}