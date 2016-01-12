#include "ScriptPrecompiled.h"
#include "VisualScriptVariable.h"

namespace Script
{
	VisualScriptVariable::VisualScriptVariable()
	{
		myType = eVisualScriptVariableTypes::IMPULSE;
	}

	VisualScriptVariable::VisualScriptVariable(int aInt)
	{
		myType = eVisualScriptVariableTypes::INT;
		myInt = aInt;
	}

	VisualScriptVariable::VisualScriptVariable(float aFloat)
	{
		myType = eVisualScriptVariableTypes::FLOAT;
		myFloat = aFloat;
	}

	VisualScriptVariable::VisualScriptVariable(bool aBool)
	{
		myType = eVisualScriptVariableTypes::BOOL;
		myBool = aBool;
	}

	VisualScriptVariable::VisualScriptVariable(const std::string& aString)
	{
		myType = eVisualScriptVariableTypes::STRING;
		myString = aString;
	}

	VisualScriptVariable::~VisualScriptVariable()
	{
	}

	void VisualScriptVariable::SetVariable(int aInt)
	{
		myType = eVisualScriptVariableTypes::INT;
		myInt = aInt;
	}

	void VisualScriptVariable::SetVariable(float aFloat)
	{
		myType = eVisualScriptVariableTypes::FLOAT;
		myFloat = aFloat;
	}

	void VisualScriptVariable::SetVariable(bool aBool)
	{
		myType = eVisualScriptVariableTypes::BOOL;
		myBool = aBool;
	}

	void VisualScriptVariable::SetVariable(const std::string& aString)
	{
		myType = eVisualScriptVariableTypes::STRING;
		myString = aString;
	}

	Script::eVisualScriptVariableTypes VisualScriptVariable::GetType() const
	{
		return myType;
	}

	void VisualScriptVariable::PushData(lua_State* aState)
	{
		switch (myType)
		{
		case Script::eVisualScriptVariableTypes::STRING:
			lua_pushstring(aState, myString.c_str());
			break;
		case Script::eVisualScriptVariableTypes::IMPULSE:
			lua_pushnil(aState);
			break;
		case Script::eVisualScriptVariableTypes::INT:
			lua_pushinteger(aState, myInt);
			break;
		case Script::eVisualScriptVariableTypes::FLOAT:
			lua_pushnumber(aState, myFloat);
			break;
		case Script::eVisualScriptVariableTypes::BOOL:
			lua_pushboolean(aState, myBool);
			break;
		default:
			break;
		}
	}
}