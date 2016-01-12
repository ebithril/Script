#pragma once

#include "VisualScriptPinInstance.h"
#include <vector>

namespace tinyxml2
{
	class XMLElement;
}

namespace Script
{
	class VisualScriptNode;
	class VisualScriptManager;

	class VisualScriptNodeInstance
	{
	public:
		VisualScriptNodeInstance();
		~VisualScriptNodeInstance();
		
		void Run(int aScriptID);

		void Load(tinyxml2::XMLElement* aElement, VisualScriptManager& aManager);

		long long GetUID();

		VisualScriptPinInstance& GetPinByIndex(int aIndex);

		void ExecutePin(const std::string& aPinName, int aScriptID);
		void GetPinData(const std::string& aPinName, lua_State* aState);
		void CallFunction(const char* aName, int aScriptID);
		bool IsStartNode();
	private:
		void CreatePins();
		std::vector<VisualScriptPinInstance> myPins;
		long long myID;
		VisualScriptNode* myNode;
	};
}