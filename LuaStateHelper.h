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

		//Returns true for success
		bool GetDataAt(std::string& aString, const int anIndex, const std::string& aCustomErrorMessage ="");
		//Returns true for success
		bool GetDataAt(float& aFloat, const int anIndex, const std::string& aCustomErrorMessage = "");
		//Returns true for success
		bool GetDataAt(int& anInt, const int anIndex, const std::string& aCustomErrorMessage = "");
		//Returns true for success
		bool GetDataAt(bool& aBool, const int anIndex, const std::string& aCustomErrorMessage = "");

		void PushData(const std::string& aString);
		void PushData(const float& aString);
		void PushData(const int& aString);
		void PushData(const bool& aString);


		int GetNumberOfArguments();

		//Returns true for success.
		bool CheckAmountOfArguments(const int anAmount, std::string aCustomErrorMessage = "");

		int ReturnError();

	private:

		std::string myFunctionName;
		lua_State*  myLuaState;

	};
}


