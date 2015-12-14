#include "LuaInterface.h"

#include <assert.h>
#include "LuaManager.h"

#include "ScriptFunctions.h"

namespace Script
{
	LuaInterface* LuaInterface::ourInstance = nullptr;
	int LuaInterface::ourRefCount = 0;
	std::function<void()> LuaInterface::ourInitFunction = nullptr;
	std::ofstream LuaInterface::ourDebugLog("../scripts/scriptLog.txt");
	std::function<void(const std::string&)> LuaInterface::ourPrintFunction = nullptr;

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
	}

	LuaInterface::~LuaInterface()
	{
		delete myLuaManager;
	}

	void LuaInterface::Print(const std::string& aString)
	{
		ourPrintFunction(aString);
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