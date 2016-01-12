#include "ScriptPrecompiled.h"
#include "VisualScriptNode.h"

#include "../include/lua.hpp"
#include "LuaInterface.h"
#include "LuaArguments.h"
#include <algorithm>

#include <assert.h>

namespace Script
{
	VisualScriptNode::VisualScriptNode()
	{
		myIsInitNode = false;

		myState = luaL_newstate();
		luaL_openlibs(myState);

		assert(myState != nullptr && "Was not able to create lua state");
	}

	VisualScriptNode::~VisualScriptNode()
	{
	}

	void VisualScriptNode::Run(const char* aCallerID, int aScriptID)
	{
		CallFunction("Run", 2, 0, LuaArguments(aCallerID, aScriptID));
	}

	void VisualScriptNode::Load(const std::string& aScriptFile)
	{
		std::ifstream file(aScriptFile);

		std::string line;

		std::getline(file, line);

		while (line != "--End_Box--")
		{
			if (line[3] == 'N') // Load node data
			{
				myName = GetName(line);

				int index = line.find("IsInitNode");

				if (index != std::string::npos)
				{
					index += 11;

					if (line[index] == '1')
					{
						myIsInitNode = true;
					}
				}
			}
			else if (line[3] == 'I') // Load input pin
			{
				VisualScriptPin pin;
				
				pin.SetName(GetName(line));
				pin.SetType(eVisualScriptPinType::INPUT);
				pin.SetVariableType(GetVariable(line));

				myPins.push_back(pin);
			}
			else if (line[3] == 'O') // Load output pin
			{
				VisualScriptPin pin;

				pin.SetName(GetName(line));
				pin.SetType(eVisualScriptPinType::OUTPUT);
				pin.SetVariableType(GetVariable(line));

				myPins.push_back(pin);
			}

			std::getline(file, line);
		}

		file.close();

		if (CheckError(luaL_loadfile(myState, aScriptFile.c_str())) == true)
		{
			LuaInterface::Print("Error in loading file look at error message above. \n");
		}
		else if (CheckError(lua_pcall(myState, 0, LUA_MULTRET, 0)) == true)
		{
			LuaInterface::Print("Error in loading file look at error message above. \n");
		}
	}

	bool VisualScriptNode::CheckError(int aResult)
	{
		if (aResult != LUA_OK)
		{
			std::string luaError = "";

			switch (aResult)
			{
			case LUA_YIELD:
				return false;
				break;
			case LUA_ERRRUN:
				luaError = "Runtime Error: ";
				break;
			case LUA_ERRSYNTAX:
				luaError = "Syntax error: ";
				break;
			case LUA_ERRMEM:
				luaError = "Memory error: ";
				break;
			case LUA_ERRGCMM:
				luaError = "Global memory error: ";
				break;
			case LUA_ERRERR:
				luaError = "Error: ";
				break;
			default:
				break;
			}
			//luaError += lua_tostring(myState, -1);
			luaError += lua_tostring(myState, 1);
			luaError += GetNiceErrorMessage(lua_tostring(myState, lua_gettop(myState)));
			luaError += '\n';

			LuaInterface::Print(luaError.c_str());

			return true;
		}
		return false;
	}

	std::vector<VisualScriptPin>& VisualScriptNode::GetPins()
	{
		return myPins;
	}

	std::string VisualScriptNode::GetNiceErrorMessage(const std::string& aLuaError)
	{
		int firstIndex = 0;
		int secondIndex = aLuaError.find_first_of(':');
		std::string fileName = aLuaError.substr(firstIndex, secondIndex);
		firstIndex = secondIndex;
		secondIndex = aLuaError.find_first_of(':', firstIndex + 1);
		std::string lineNumber = aLuaError.substr(firstIndex + 1, secondIndex - firstIndex - 1);
		firstIndex = secondIndex;
		secondIndex = aLuaError.length();
		std::string errorMsg = aLuaError.substr(firstIndex + 1, secondIndex - firstIndex - 1);

		std::string output = "File: ";
		output += fileName + " Line: " + lineNumber + " Error Message: " + errorMsg;

		return output;
	}

	std::string VisualScriptNode::GetName(const std::string& aLine)
	{
		int index = aLine.find("Name");

		if (index == std::string::npos)
		{
			assert(false && "Node doesn't have a name");
			return "";
		}

		index += 5;

		int secondIndex = aLine.find(']', index);

		std::string name = aLine.substr(index, secondIndex - index);

		std::transform(name.begin(), name.end(), name.begin(), std::tolower);

		return name;
	}

	const std::string& VisualScriptNode::GetName() const
	{
		return myName;
	}

	eVisualScriptVariableTypes VisualScriptNode::GetVariable(const std::string& aLine)
	{
		int index = aLine.find("VarType");

		if (index == std::string::npos)
		{
			assert(false && "Pin doesn't have a variable type");
			return eVisualScriptVariableTypes::IMPULSE;
		}

		index += 8;

		int secondIndex = aLine.find(']', index);

		std::string variableName = aLine.substr(index, secondIndex - index);

		if (variableName == "impulse")
		{
			return eVisualScriptVariableTypes::IMPULSE;
		}
		else if (variableName == "string")
		{
			return eVisualScriptVariableTypes::STRING;
		}
		else if (variableName == "float")
		{
			return eVisualScriptVariableTypes::FLOAT;
		}
		else if (variableName == "int")
		{
			return eVisualScriptVariableTypes::INT;
		}
		else if (variableName == "bool")
		{
			return eVisualScriptVariableTypes::BOOL;
		}
		else
		{
			assert(false && "Non supported variable type");
		}

		return eVisualScriptVariableTypes::IMPULSE;
	}

	void VisualScriptNode::CallFunction(const char* aName, const char* aID, int aScriptID)
	{
		CallFunction(aName, 2, 0, LuaArguments(aID, aScriptID));
	}

	void VisualScriptNode::CallFunction(const char* aName, int aNumberOfArguments, int aNumberOfReturns, const LuaArguments& someArguments)
	{
		int type = lua_getglobal(myState, aName);

		if (type != LUA_TFUNCTION)
		{
			std::string errorMsg = aName;
			errorMsg += " is a ";
			errorMsg += lua_typename(myState, type);
			errorMsg += " and not a function please fix this or talk to programmer.";

			LuaInterface::Print(errorMsg.c_str());
			lua_pop(myState, 1);

			return;
		}

		someArguments.PushArguments(myState, aNumberOfArguments);

		CheckError(lua_pcall(myState, aNumberOfArguments, aNumberOfReturns, 0));
	}

	bool VisualScriptNode::IsStartNode()
	{
		return myIsInitNode;
	}

	void VisualScriptNode::RegisterFunction(const char* aName, lua_CFunction aFunction)
	{
		lua_register(myState, aName, aFunction);
	}
}