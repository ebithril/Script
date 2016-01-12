#pragma once

class LuaArguments;

namespace Script
{
	class LuaBaseScript
	{
	public:
		LuaBaseScript();
		~LuaBaseScript();

		virtual void CallFunction(const char* aName, int aNumberOfArgs, int aNumberOfReturns, const LuaArguments& someArguments) = 0;
	};
}