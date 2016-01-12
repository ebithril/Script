#pragma once

struct lua_State;

namespace Script
{
	int ExecutePin(lua_State* aState);

	int GetPinData(lua_State* aState);
}