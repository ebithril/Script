#pragma once

#include "VisualScriptVariable.h"
#include "VisualScriptingPinConnection.h"

namespace Script
{
	class VisualScriptPin;
	class VisualScriptNodeInstance;

	class VisualScriptPinInstance
	{
	public:
		VisualScriptPinInstance();
		~VisualScriptPinInstance();

		void SetVariable(const VisualScriptVariable& aVariable);
		void SetConnection(const VisualScriptingPinConnection& aConnection);

		void ExecutePin(int aScriptID);
		void SetPin(VisualScriptPin* pin);
		void SetNode(VisualScriptNodeInstance* aNodeInstance);
		const std::string& GetName();
		void GetPinData(lua_State* aState);
		eVisualScriptVariableTypes GetPinType();
	private:
		VisualScriptVariable myVariable;
		VisualScriptingPinConnection myConnection;
		VisualScriptPin* myPin;
		VisualScriptNodeInstance* myNode;
	};
}