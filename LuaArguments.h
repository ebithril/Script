#pragma once

#include "LuaArgument.h"

struct lua_State;

class LuaArguments
{
public:
	LuaArguments();
	LuaArguments(LuaArgument aArg);
	LuaArguments(LuaArgument aArg1, LuaArgument aArg2);
	LuaArguments(LuaArgument aArg1, LuaArgument aArg2, LuaArgument aArg3);
	LuaArguments(LuaArgument aArg1, LuaArgument aArg2, LuaArgument aArg3, LuaArgument aArg4);
	LuaArguments(LuaArgument aArg1, LuaArgument aArg2, LuaArgument aArg3, LuaArgument aArg4, LuaArgument aArg5);

	void PushArguments(lua_State* aState, int aNumberOfArgs);

	~LuaArguments();

private:
	LuaArgument myArguments[5];
};