#include "ScriptPrecompiled.h"
#include "VisualScriptPin.h"

namespace Script
{
	VisualScriptPin::VisualScriptPin()
	{
	}

	VisualScriptPin::~VisualScriptPin()
	{
	}

	void VisualScriptPin::SetVariableType(eVisualScriptVariableTypes aVariableType)
	{
		myVariableType = aVariableType;
	}

	void VisualScriptPin::SetType(eVisualScriptPinType aVariableType)
	{
		myType = aVariableType;
	}

	void VisualScriptPin::SetName(const std::string& aName)
	{
		myName = aName;
	}

	Script::eVisualScriptPinType VisualScriptPin::GetPinType() const
	{
		return myType;
	}

	Script::eVisualScriptVariableTypes VisualScriptPin::GetVariableType() const
	{
		return myVariableType;
	}

	const std::string& VisualScriptPin::GetName() const
	{
		return myName;
	}
}