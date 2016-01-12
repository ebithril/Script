#include "ScriptPrecompiled.h"
#include "VisualScriptManager.h"

#include <string>
#include "ScriptFunctions.h"

namespace Script
{
	VisualScriptManager::VisualScriptManager()
	{
		myNextStateID = 1;
	}

	VisualScriptManager::~VisualScriptManager()
	{
	}

	void VisualScriptManager::LoadAllNodes()
	{
		std::vector<std::string> scriptFiles = GetFilesWithExtension("../Resources/Script/Nodes/", ".lua");

		for (const std::string& scriptFile : scriptFiles)
		{
			if (scriptFile != "")
			{
				myNodes.push_back(VisualScriptNode());
				myNodes.back().Load(scriptFile);
			}
		}
	}

	VisualScriptNode* VisualScriptManager::GetNode(const std::string& aNodeName)
	{
		for (VisualScriptNode& node : myNodes)
		{
			if (node.GetName() == aNodeName)
			{
				return &node;
			}
		}

		return nullptr;
	}

	void VisualScriptManager::ExecutePin(const std::string& pinName, long long callerID, int scriptID)
	{
		for (std::shared_ptr<VisualScript>& script : myScripts)
		{
			if (script->GetID() == scriptID)
			{
				script->ExecutePin(pinName, long long(callerID));
			}
		}
	}

	void VisualScriptManager::GetPinData(const std::string& aPinName, long long aCallerID, int aScriptID, lua_State* aState)
	{
		for (std::shared_ptr<VisualScript>& script : myScripts)
		{
			if (script->GetID() == aScriptID)
			{
				script->GetPinData(aPinName, long long(aCallerID), aState);
			}
		}
	}

 	void VisualScriptManager::RegisterCallback(const std::string& cppFunctionName, const std::string& luaFunctionName, int scriptId, long long aCallerID)
	{
		for (std::shared_ptr<VisualScript>& script : myScripts)
		{
			if (script->GetID() == scriptId)
			{
				myRegisterCallbackFunctions[cppFunctionName](luaFunctionName, script, aCallerID);

				return;
			}
		}
	}

	std::shared_ptr<VisualScript> VisualScriptManager::LoadVisualScript(const std::string& aFileName)
	{
		std::shared_ptr<VisualScript> newScript(new VisualScript);

		newScript->Load(aFileName, *this);

		newScript->SetID(myNextStateID);
		myNextStateID += 2;

		myScripts.push_back(newScript);

		return newScript;
	}

	void VisualScriptManager::AddCallbackFunction(const std::string& aName, std::function<void(std::string, std::shared_ptr<LuaBaseScript>, long long)> aFunction)
	{
		myRegisterCallbackFunctions[aName] = aFunction;
	}

	void VisualScriptManager::RegisterFunction(const char* aName, lua_CFunction aFunction)
	{
		for (VisualScriptNode& node : myNodes)
		{
			node.RegisterFunction(aName, aFunction);
		}
	}

}