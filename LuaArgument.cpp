#include "LuaArgument.h"

#include "../include/lua.hpp"

LuaArgument::LuaArgument()
{
	myArgumentType = eLuaArgumentType::eNIL;
}

LuaArgument::LuaArgument(const char* aString)
{
	myArgumentType = eLuaArgumentType::eSTRING;
	myString = aString;
}

LuaArgument::LuaArgument(int aNumber)
{
	myArgumentType = eLuaArgumentType::eNUMBER;
	myNumber = static_cast<float>(aNumber);
}

LuaArgument::LuaArgument(double aNumber)
{
	myArgumentType = eLuaArgumentType::eNUMBER;
	myNumber = static_cast<float>(aNumber);
}

LuaArgument::LuaArgument(float aNumber)
{
	myArgumentType = eLuaArgumentType::eNUMBER;
	myNumber = aNumber;
}

LuaArgument::LuaArgument(bool aBool)
{
	myArgumentType = eLuaArgumentType::eBOOL;
	myBool = aBool;
}

LuaArgument::LuaArgument(void* aLightUserData)
{
	myArgumentType = eLuaArgumentType::eLIGHTUSERDATA;
	myLightUserData = aLightUserData;
}

LuaArgument::~LuaArgument()
{
}

void LuaArgument::PushArgument(lua_State* aState) const
{
	switch (myArgumentType)
	{
	case eLuaArgumentType::eSTRING:
		lua_pushstring(aState, myString);
		break;
	case eLuaArgumentType::eNUMBER:
		lua_pushnumber(aState, myNumber);
		break;
	case eLuaArgumentType::eBOOL:
		lua_pushboolean(aState, myBool);
		break;
	case eLuaArgumentType::eLIGHTUSERDATA:
		lua_pushlightuserdata(aState, myLightUserData);
		break;
	case eLuaArgumentType::eNIL:
		lua_pushnil(aState);
		break;
	default:
		break;
	}
}
