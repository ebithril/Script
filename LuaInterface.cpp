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

	void LuaInterface::UseFile(const std::string& aFilePath)
	{
		myLuaManager->UseFile(aFilePath.c_str());
	}

	void LuaInterface::RegisterFunction(const std::string& aName, const lua_CFunction& aFunction, const std::string& aDescription)
	{
		myLuaManager->RegisterFunction(aName.c_str(), aFunction, aDescription.c_str());
	}

	void LuaInterface::CallFunction(const std::string& aName, int aNumberOfArgs, int aNumberOfReturns)
	{
		myLuaManager->CallFunction(aName.c_str(), aNumberOfArgs, aNumberOfReturns);
	}

	void LuaInterface::CallString(const std::string& aString)
	{
		myLuaManager->CallString(aString.c_str());
	}

	std::vector<FunctionInformation> LuaInterface::GetFunctionInfo()
	{
		return myLuaManager->GetFunctionInfo();
	}

	LuaInterface::LuaInterface()
	{
		myLuaManager = new LuaManager();
	}

	LuaInterface::~LuaInterface()
	{
		delete myLuaManager;
	}

	LuaInterface* CreateLuaInterface()
	{
		if (LuaInterface::ourRefCount == 0)
		{
			++LuaInterface::ourRefCount;

			//assert(LuaInterface::ourInitFunction != nullptr && "Cannot create luainterface since no registerfunction is set");
			
			LuaInterface::ourDebugLog.open("../scripts/scriptLog.txt");

			PrintLog("created system");

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