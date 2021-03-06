#include "ScriptPrecompiled.h"
#include "LuaInterface.h"

#include <assert.h>
#include "LuaManager.h"

#include "ScriptFunctions.h"
#include "VisualScriptFunctions.h"
#include "VisualScriptManager.h"

namespace Script
{
	LuaInterface* LuaInterface::ourInstance = nullptr;
	int LuaInterface::ourRefCount = 0;
	std::function<void()> LuaInterface::ourInitFunction = nullptr;
	std::function<void(const std::string&)> LuaInterface::ourPrintFunction = nullptr;
	std::ofstream LuaInterface::ourDebugLog;

	void LuaInterface::RegisterAddCallBackFunction(const std::string& aName, std::function<void(std::string, std::shared_ptr<LuaBaseScript>, long long)> aFunction)
	{
		myLuaManager->RegisterAddCallBackFunction(aName, aFunction);
		myVisualScriptManager->AddCallbackFunction(aName, aFunction);
	}

	void LuaInterface::Release()
	{
		--ourRefCount;

		if (ourRefCount <= 0)
		{
			delete ourInstance;

			ourDebugLog.close();

			ourInstance = nullptr;
		}
	}

	void LuaInterface::Update()
	{
		myLuaManager->Update();
	}

	void LuaInterface::RegisterFunction(const std::string& aName, const lua_CFunction& aFunction, const std::string& aDescription)
	{
		myLuaManager->RegisterFunction(aName.c_str(), aFunction, aDescription.c_str());
		myVisualScriptManager->RegisterFunction(aName.c_str(), aFunction);
	}

	std::shared_ptr<LuaState> LuaInterface::CreateLuaState()
	{
		return myLuaManager->CreateLuaState();
	}

	std::vector<FunctionInformation> LuaInterface::GetFunctionInfo()
	{
		return myLuaManager->GetFunctionInfo();
	}

	void LuaInterface::SetPrintFunction(std::function<void(const std::string&)> aPrintFunction)
	{
		ourPrintFunction = aPrintFunction;
	}

	LuaInterface::LuaInterface()
	{
		myLuaManager = new LuaManager();
		myVisualScriptManager = new VisualScriptManager();
		myVisualScriptManager->LoadAllNodes();

		myVisualScriptManager->RegisterFunction("ExecutePin", Script::ExecutePin);
		myVisualScriptManager->RegisterFunction("GetPinData", Script::GetPinData);

		myVisualScriptManager->RegisterFunction("Print", Script::PrintLog);
		myVisualScriptManager->RegisterFunction("RegisterCallback", Script::RegisterCallback);

		ourDebugLog.open("/Data/Script/ScriptLog.txt", std::ios_base::out);
	}

	LuaInterface::~LuaInterface()
	{
		ourDebugLog.close();
		delete myLuaManager;
	}

	void LuaInterface::RegisterCallback(const std::string& cppFunctionName, const std::string& luaFunctionName, int scriptId, long long aGameObjectID)
	{
		if (scriptId % 2 == 0)
		{
			myLuaManager->RegisterCallback(cppFunctionName, luaFunctionName, scriptId, aGameObjectID);
		}
		else
		{
			myVisualScriptManager->RegisterCallback(cppFunctionName, luaFunctionName, scriptId, aGameObjectID);
		}
	}

	std::shared_ptr<VisualScript> LuaInterface::LoadVisualScript(const std::string& aFileName)
	{
		return myVisualScriptManager->LoadVisualScript(aFileName);
	}

	void LuaInterface::Print(const std::string& aString)
	{
		ourPrintFunction(aString);
	}

	void LuaInterface::ExecutePin(const std::string& pinName, long long callerID, int scriptID)
	{
		myVisualScriptManager->ExecutePin(pinName, callerID, scriptID);
	}

	void LuaInterface::GetPinData(const std::string& aPinName, long long aCallerID, int aScriptID, lua_State* aState)
	{
		myVisualScriptManager->GetPinData(aPinName, aCallerID, aScriptID, aState);
	}

	LuaInterface* CreateLuaInterface()
	{
		if (LuaInterface::ourRefCount == 0)
		{
			++LuaInterface::ourRefCount;

			LuaInterface::ourDebugLog.open("../scripts/scriptLog.txt");

			if (LuaInterface::ourPrintFunction == nullptr)
			{
				LuaInterface::ourPrintFunction = [](const std::string& aString){ PrintLog(aString); };
			}

			LuaInterface::Print("created system");

			LuaInterface::ourInstance = new LuaInterface();
			LuaInterface::ourInstance->myLuaManager->Init(LuaInterface::ourInitFunction);
			--LuaInterface::ourRefCount;
		}

		++LuaInterface::ourRefCount;

		return LuaInterface::ourInstance;
	}

	void SetInitFunction(const std::function<void()>& aFunction)
	{
		LuaInterface::ourInitFunction = aFunction;
	}
}