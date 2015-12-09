#include "LuaManager.h"

#include "LuaState.h"

#include "file_watcher.h"

#include <lua.hpp>
#include <functional>
#include <assert.h>

#include <windows.h>
#include <string.h>

#include "ScriptFunctions.h"
#include <iostream>

namespace Script
{
	void LuaManager::Init(const std::function<void()>& aFunction)
	{
		delete myState;
		myState = new LuaState();

		lua_CFunction logFunction = (lua_CFunction)&PrintLog;

		myState->RegisterFunction("Print", logFunction, "Print a message to the log");

		aFunction();
	}

	void LuaManager::Update()
	{
		CFileWatcher::GetInstance()->FlushChanges();

		myState->CallFunction("Update", 0, 0);
	}

	void LuaManager::UseFile(const char* aFilePath)
	{
		myState->UseFile(aFilePath);
	}

	void LuaManager::RegisterFunction(const char* aName, const lua_CFunction& aFunction, const char* aDescription)
	{
		myState->RegisterFunction(aName, aFunction, aDescription);
	}

	LuaManager::LuaManager()
	{
		myState = nullptr;
	}

	LuaManager::~LuaManager()
	{
		delete myState;
	}
}