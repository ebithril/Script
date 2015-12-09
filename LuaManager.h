#pragma once

struct lua_State;

namespace std
{
	template <typename Type>
	class function;
}
typedef int(*lua_CFunction) (lua_State *L);

namespace Script
{
	class LuaInterface;
	class LuaState;

	class LuaManager
	{
	friend LuaInterface;

	public:
		void Init(const std::function<void()>& aFunction);

		void Update();
		void UseFile(const char* aFilePath);
		void RegisterFunction(const char* aName, const lua_CFunction& aFunction, const char* aDescription);
	private:
		LuaManager();
		~LuaManager();

		LuaState* myState;
	};
}