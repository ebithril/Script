#pragma once

struct lua_State;

enum class eLuaArgumentType
{
	eSTRING,
	eNUMBER,
	eBOOL,
	eLIGHTUSERDATA,
	eNIL
};

class LuaArgument
{
public:
	LuaArgument();
	LuaArgument(const char* aString);
	LuaArgument(float aNumber);
	LuaArgument(bool aBool);
	LuaArgument(void* aLightUserData);
	~LuaArgument();

	void PushArgument(lua_State* aState);
private:
	union
	{
		float myNumber;
		bool myBool;
		void* myLightUserData;
		const char* myString;
	};
	eLuaArgumentType myArgumentType;
};