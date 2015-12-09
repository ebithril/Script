#pragma once

struct lua_State;

namespace Script
{
	int PrintLog(lua_State* aState);
	int PrintLog(const char* aStringToPrint);
}