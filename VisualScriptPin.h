#pragma once

#include "VisualScriptVariableTypes.h"
#include "VisualScriptPinType.h"
#include <string>

namespace Script
{
	class VisualScriptPin
	{
	public:
		VisualScriptPin();
		~VisualScriptPin();

		void SetVariableType(eVisualScriptVariableTypes aVariableType);
		void SetType(eVisualScriptPinType aVariableType);
		void SetName(const std::string& aName);

		eVisualScriptPinType GetPinType() const;
		eVisualScriptVariableTypes GetVariableType() const;
		const std::string& GetName() const;

	private:
		eVisualScriptVariableTypes myVariableType;
		eVisualScriptPinType myType;
		std::string myName;
	};
}