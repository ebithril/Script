#pragma once
#include <string>

struct lua_State;

namespace Script
{

	class LuaStateHelper
	{
	public:
		LuaStateHelper(lua_State* aLuaState, const std::string& aFunctionName);
		LuaStateHelper() = delete;
		~LuaStateHelper();

		void GetDataAt(std::string& aString, const int anIndex, const std::string& aCustomErrorMessage ="");
		void GetDataAt(float& aFloat, const int anIndex, const std::string& aCustomErrorMessage = "");
		void GetDataAt(int& anInt, const int anIndex, const std::string& aCustomErrorMessage = "");
		void GetDataAt(bool& aBool, const int anIndex, const std::string& aCustomErrorMessage = "");

		void PushData(const std::string& aString);
		void PushData(const float& aString);
		void PushData(const int& aString);
		void PushData(const bool& aString);

		int GetNumberOfArguments();

		void CheckAmountOfArguments(const int anAmount, std::string aCustomErrorMessage = "");


	private:
		void ThrowError(const std::string& anErrorText);

		std::string myFunctionName;
		lua_State*  myLuaState;

	};
}


