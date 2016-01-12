#include "ScriptPrecompiled.h"
#include "VisualScriptPinInstance.h"

#include "VisualScriptPin.h"
#include "VisualScriptNodeInstance.h"

#include <assert.h>

#include "../include/lua.hpp"

namespace Script
{
	VisualScriptPinInstance::VisualScriptPinInstance()
	{
	}

	VisualScriptPinInstance::~VisualScriptPinInstance()
	{
	}

	void VisualScriptPinInstance::SetVariable(const VisualScriptVariable& aVariable)
	{
		assert(aVariable.GetType() == myPin->GetVariableType() && "Variable type not same as pin type please contact programmer ;)");

		myVariable = aVariable;
	}

	void VisualScriptPinInstance::SetConnection(const VisualScriptingPinConnection& aConnection)
	{
		myConnection = aConnection;
	}

	void VisualScriptPinInstance::ExecutePin(int aScriptID)
	{
		if (myPin->GetPinType() == eVisualScriptPinType::OUTPUT)
		{
			if (myConnection.myTargetPin != nullptr)
			{
				if (myPin->GetVariableType() == eVisualScriptVariableTypes::IMPULSE)
				{
					myConnection.myTargetPin->ExecutePin(aScriptID);
				}
				else
				{
					myConnection.myTargetPin->SetVariable(myVariable);
				}
			}
		}
		else
		{
			myNode->Run(aScriptID);
		}
	}

	void VisualScriptPinInstance::SetPin(VisualScriptPin* pin)
	{
		myPin = pin;
	}

	void VisualScriptPinInstance::SetNode(VisualScriptNodeInstance* aNodeInstance)
	{
		myNode = aNodeInstance;
	}

	const std::string& VisualScriptPinInstance::GetName()
	{
		return myPin->GetName();
	}

	void VisualScriptPinInstance::GetPinData(lua_State* aState)
	{
		myVariable.PushData(aState);
	}

	eVisualScriptVariableTypes VisualScriptPinInstance::GetPinType()
	{
		return myPin->GetVariableType();
	}

}