#pragma once

#include "VisualScriptVariableTypes.h"
#include <string>

namespace Script
{
	class VisualScriptVariable
	{
	public:
		VisualScriptVariable();
		VisualScriptVariable(int aInt);
		VisualScriptVariable(float aFloat);
		VisualScriptVariable(bool aBool);
		VisualScriptVariable(const std::string& aString);

		~VisualScriptVariable();

		void SetVariable(int aInt);
		void SetVariable(float aFloat);
		void SetVariable(bool aBool);
		void SetVariable(const std::string& aString);

		eVisualScriptVariableTypes GetType() const;
		void PushData(lua_State* aState);
	private:
		eVisualScriptVariableTypes myType;

		union
		{
			int myInt;
			float myFloat;
			bool myBool;
		};
		
		std::string myString;
	};
}