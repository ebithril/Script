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

	void LuaInterface::UseFile(const std::string& aFilePath)
	{
		myLuaManager->UseFile(aFilePath.c_str());
	}

	void LuaInterface::RegisterFunction(const std::string& aName, const lua_CFunction& aFunction, const std::string& aDescription)
	{
		myLuaManager->RegisterFunction(aName.c_str(), aFunction, aDescription.c_str());
	}

	void LuaInterface::CallFunction(const std::string& aName, int aNumberOfArgs, int aNumberOfReturns, LuaArguments& someArguments)
	{
		myLuaManager->CallFunction(aName.c_str(), aNumberOfArgs, aNumberOfReturns, someArguments);
	}

	void LuaInterface::CallString(const std::string& aString)
	{
		myLuaManager->CallString(aString.c_str());
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

		myLuaManager->Init(ourInitFunction);
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
			//assert(LuaInterface::ourInitFunction != nullptr && "Cannot create luainterface since no registerfunction is set");
			
			LuaInterface::ourDebugLog.open("../scripts/scriptLog.txt");

			if (LuaInterface::ourPrintFunction == nullptr)
			{
				LuaInterface::ourPrintFunction = [](const std::string& aString){ PrintLog(aString); };
			}

			LuaInterface::Print("created system");

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