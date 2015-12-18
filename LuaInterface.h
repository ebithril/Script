#pragma once

#include <functional>
#include <fstream>
#include <vector>
#include <memory>
#include "FunctionInformation.h"

#define LUA_TRUE 0
#define LUA_FALSE 1

struct lua_State;

namespace std
{
	template <typename Type>
	class function;
}

typedef int(*lua_CFunction) (lua_State *L);

class LuaArguments;


namespace Script
{
	class LuaManager;
	class LuaState;

	class LuaInterface
	{
	friend LuaInterface* CreateLuaInterface();
	friend void SetInitFunction(const std::function<void()>& aFunction);

	public:
		void Release();

		void Update();
		void RegisterFunction(const std::string& aName, const lua_CFunction& aFunction, const std::string& aDescription);
		std::shared_ptr<LuaState> CreateLuaState();

		std::vector<FunctionInformation> GetFunctionInfo();

		static void SetPrintFunction(std::function<void(const std::string&)> aPrintFunction);
		static void Print(const std::string& aString);

		static std::ofstream ourDebugLog;
		void RegisterAddCallBackFunction(const std::string& aName, std::function<void(std::string, std::shared_ptr<LuaState>, int)> aFunction);

		//Used internally do not abuse!
		void RegisterCallback(const std::string& cppFunctionName, const std::string& luaFunctionName, int scriptId, int aGameObjectID);

	private:
		static std::function<void(const std::string&)> ourPrintFunction;
		
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