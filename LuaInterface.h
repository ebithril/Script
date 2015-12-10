#pragma once

#include <functional>
#include <fstream>
#include <vector>
#include "FunctionInformation.h"

struct lua_State;

namespace std
{
	template <typename Type>
	class function;
}

typedef int(*lua_CFunction) (lua_State *L);


namespace Script
{
	class LuaManager;

	class LuaInterface
	{
	friend LuaInterface* CreateLuaInterface();
	friend void SetInitFunction(const std::function<void()>& aFunction);

	public:
		void Release();

		void Update();
		void UseFile(const std::string& aFilePath);
		void RegisterFunction(const std::string& aName, const lua_CFunction& aFunction, const std::string& aDescription);
		void CallFunction(const std::string& aName, int aNumberOfArgs, int aNumberOfReturns);

		void CallString(const std::string& aString);

		std::vector<FunctionInformation> GetFunctionInfo();

		static std::ofstream ourDebugLog;
	private:
		LuaInterface();
		~LuaInterface();

		LuaManager* myLuaManager;

		static LuaInterface* ourInstance;
		static int ourRefCount;
		static std::function<void()> ourInitFunction;
	};

	LuaInterface* CreateLuaInterface();
	void SetInitFunction(const std::function<void()>& aFunction);
}