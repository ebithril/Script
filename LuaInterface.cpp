#include "LuaInterface.h"

#include <assert.h>
#include "LuaManager.h"

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

	LuaInterface::LuaInterface()
	{
		myLuaManager = new LuaManager();

		myLuaManager->Init(ourInitFunction);
	}

	LuaInterface::~LuaInterface()
	{
		delete myLuaManager;
	}

	LuaInterface* CreateLuaInterface()
	{
		if (LuaInterface::ourRefCount == 0)
		{
			assert(LuaInterface::ourInitFunction != nullptr && "Cannot create luainterface since no registerfunction is set");

			LuaInterface::ourInstance = new LuaInterface();
		}

		++LuaInterface::ourRefCount;

		return LuaInterface::ourInstance;
	}

	void SetInitFunction(const std::function<void()>& aFunction)
	{
		LuaInterface::ourInitFunction = aFunction;
	}
}