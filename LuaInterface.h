#pragma once

#include <functional>
#include <fstream>

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