#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "VisualScriptNode.h"
#include "VisualScript.h"

namespace Script
{
	class VisualScriptManager
	{
	public:
		VisualScriptManager();
		~VisualScriptManager();

		void LoadAllNodes();
		VisualScriptNode* GetNode(const std::string& aNodeName);
		void ExecutePin(const std::string& pinName, long long callerID, int scriptID);
		void GetPinData(const std::string& aPinName, long long aCallerID, int aScriptID, lua_State* aState);
		void RegisterCallback(const std::string& cppFunctionName, const std::string& luaFunctionName, int scriptId, long long aCallerID);
		std::shared_ptr<VisualScript> LoadVisualScript(const std::string& aFileName);

		void AddCallbackFunction(const std::string& aName, std::function<void(std::string, std::shared_ptr<LuaBaseScript>, long long)> aFunction);

		void RegisterFunction(const char* aName, lua_CFunction aFunction);
	private:
		std::vector<VisualScriptNode> myNodes;
		std::vector<std::shared_ptr<VisualScript>> myScripts;
		std::unordered_map<std::string, std::function<void(std::string, std::shared_ptr<LuaBaseScript>, long long)>> myRegisterCallbackFunctions;

		int myNextStateID;
	};
}