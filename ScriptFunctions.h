#pragma once

#include <string>

struct lua_State;

namespace Script
{
	int PrintLog(lua_State* aState);
	void PrintLog(const std::string& aStringToPrint);
	int RegisterCallback(lua_State* aState);
}