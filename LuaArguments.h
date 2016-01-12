#pragma once

#include "LuaArgument.h"

struct lua_State;

class LuaArguments
{
public:
	LuaArguments();
	LuaArguments(const LuaArgument& aArg);
	LuaArguments(const LuaArgument& aArg1, const LuaArgument& aArg2);
	LuaArguments(const LuaArgument& aArg1, const LuaArgument& aArg2, const LuaArgument& aArg3);
	LuaArguments(const LuaArgument& aArg1, const LuaArgument& aArg2, const LuaArgument& aArg3, const LuaArgument& aArg4);
	LuaArguments(const LuaArgument& aArg1, const LuaArgument& aArg2, const LuaArgument& aArg3, const LuaArgument& aArg4, const LuaArgument& aArg5);

	void PushArguments(lua_State* aState, int aNumberOfArgs) const;

	const LuaArgument& GetArgumentFromIndex(short aIndex) const;

	~LuaArguments();

private:
	LuaArgument myArguments[5];
};