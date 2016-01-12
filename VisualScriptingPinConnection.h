#pragma once

#include "VisualScriptVariableTypes.h"

namespace Script
{
	class VisualScriptPinInstance;
	
	struct VisualScriptingPinConnection
	{
		VisualScriptPinInstance* myTargetPin = nullptr;
		eVisualScriptVariableTypes myType = eVisualScriptVariableTypes::IMPULSE;
	};
}