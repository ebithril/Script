#pragma once

#include <string>
#include <vector>

struct lua_State;

namespace Script
{
	int PrintLog(lua_State* aState);
	void PrintLog(const std::string& aStringToPrint);
	int RegisterCallback(lua_State* aState);
	std::vector<std::string> GetFilesWithExtension(const std::string& aDirectory, const std::string& anExtension);
}