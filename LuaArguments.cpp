#include "LuaArguments.h"

#include <assert.h>

LuaArguments::LuaArguments()
{
}

LuaArguments::LuaArguments(LuaArgument aArg)
{
	myArguments[0] = aArg;
}

LuaArguments::LuaArguments(LuaArgument aArg1, LuaArgument aArg2)
{
	myArguments[0] = aArg1;
	myArguments[1] = aArg2;

}

LuaArguments::LuaArguments(LuaArgument aArg1, LuaArgument aArg2, LuaArgument aArg3)
{
	myArguments[0] = aArg1;
	myArguments[1] = aArg2;
	myArguments[2] = aArg3;

}

LuaArguments::LuaArguments(LuaArgument aArg1, LuaArgument aArg2, LuaArgument aArg3, LuaArgument aArg4)
{
	myArguments[0] = aArg1;
	myArguments[1] = aArg2;
	myArguments[2] = aArg3;
	myArguments[3] = aArg4;

}

LuaArguments::LuaArguments(LuaArgument aArg1, LuaArgument aArg2, LuaArgument aArg3, LuaArgument aArg4, LuaArgument aArg5)
{
	myArguments[0] = aArg1;
	myArguments[1] = aArg2;
	myArguments[2] = aArg3;
	myArguments[3] = aArg4;
	myArguments[4] = aArg5;
}

void LuaArguments::PushArguments(lua_State* aState, int aNumberOfArgs)
{
	if (aNumberOfArgs > 5 || aNumberOfArgs < 0)
	{
		assert(false && "Cannot handle this many args");
	}

	for (int i = 0; i < aNumberOfArgs; i++)
	{
		myArguments[i].PushArgument(aState);
	}
}

LuaArguments::~LuaArguments()
{
}
