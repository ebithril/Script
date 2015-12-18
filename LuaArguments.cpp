#include "LuaArguments.h"

#include <assert.h>

LuaArguments::LuaArguments()
{
}

LuaArguments::LuaArguments(const LuaArgument& aArg)
{
	myArguments[0] = aArg;
}

LuaArguments::LuaArguments(const LuaArgument& aArg1, const LuaArgument& aArg2)
{
	myArguments[0] = aArg1;
	myArguments[1] = aArg2;

}

LuaArguments::LuaArguments(const LuaArgument& aArg1, const LuaArgument& aArg2, const LuaArgument& aArg3)
{
	myArguments[0] = aArg1;
	myArguments[1] = aArg2;
	myArguments[2] = aArg3;

}

LuaArguments::LuaArguments(const LuaArgument& aArg1, const LuaArgument& aArg2, const LuaArgument& aArg3, const LuaArgument& aArg4)
{
	myArguments[0] = aArg1;
	myArguments[1] = aArg2;
	myArguments[2] = aArg3;
	myArguments[3] = aArg4;

}

LuaArguments::LuaArguments(const LuaArgument& aArg1, const LuaArgument& aArg2, const LuaArgument& aArg3, const LuaArgument& aArg4, const LuaArgument& aArg5)
{
	myArguments[0] = aArg1;
	myArguments[1] = aArg2;
	myArguments[2] = aArg3;
	myArguments[3] = aArg4;
	myArguments[4] = aArg5;
}

void LuaArguments::PushArguments(lua_State* aState, int aNumberOfArgs) const
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
