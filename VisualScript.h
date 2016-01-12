#pragma once

#include "LuaBaseScript.h"
#include "VisualScriptNodeInstance.h"
#include <vector>

namespace Script
{
	class VisualScriptManager;

	class VisualScript : public LuaBaseScript
	{
	public:
		VisualScript();
		~VisualScript();

		void SetID(int aID);
		void Load(const std::string& aFile, VisualScriptManager& aManager);
		
		void CallFunction(const char* aName, int aNumberOfArgs, int aNumberOfReturns, const LuaArguments& someArguments) override;

		void ExecutePin(const std::string& aPinName, long long aNodeID);
		void GetPinData(const std::string& aPinName, long long aCallerID, lua_State* aState);
		int GetID();
		void Start();
	private:
		VisualScriptNodeInstance& GetNodeFromID(long long aID);
		void LoadConnection(VisualScriptNodeInstance& aStartNode, tinyxml2::XMLElement* aConnection);
		void LoadInputConnection(VisualScriptNodeInstance& aStartNode, tinyxml2::XMLElement* aConnection);
		void LoadOutputConnection(VisualScriptNodeInstance& aStartNode, tinyxml2::XMLElement* aConnection);

		int myScriptID;
		std::vector<VisualScriptNodeInstance> myNodes;
	};
}