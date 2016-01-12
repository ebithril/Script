#include "ScriptPrecompiled.h"
#include "VisualScriptNodeInstance.h"

#include "VisualScriptNode.h"
#include "VisualScriptManager.h"

#include "tinyxml2.h"

namespace Script
{
	VisualScriptNodeInstance::VisualScriptNodeInstance()
	{
	}

	VisualScriptNodeInstance::~VisualScriptNodeInstance()
	{
	}

	void VisualScriptNodeInstance::Run(int aScriptID)
	{
		std::string id = std::to_string(myID);

		myNode->Run(id.c_str(), aScriptID);
	}

	void VisualScriptNodeInstance::Load(tinyxml2::XMLElement* aElement, VisualScriptManager& aManager)
	{
		myNode = aManager.GetNode(aElement->FirstChildElement("Name")->GetText());

		CreatePins();

		std::string uid = aElement->FirstChildElement("UID")->GetText();

		myID = std::stoll(uid);
	}

	long long VisualScriptNodeInstance::GetUID()
	{
		return myID;
	}

	VisualScriptPinInstance& VisualScriptNodeInstance::GetPinByIndex(int aIndex)
	{
		return myPins[aIndex];
	}

	void VisualScriptNodeInstance::ExecutePin(const std::string& aPinName, int aScriptID)
	{
		for (VisualScriptPinInstance& pin : myPins)
		{
			if (pin.GetName() == aPinName)
			{
				pin.ExecutePin(aScriptID);
			}
		}
	}

	void VisualScriptNodeInstance::CreatePins()
	{
		std::vector<VisualScriptPin>& pins = myNode->GetPins();

		for (VisualScriptPin& pin : pins)
		{
			VisualScriptPinInstance pinInstance;
			pinInstance.SetPin(&pin);
			pinInstance.SetNode(this);

			myPins.push_back(pinInstance);
		}
	}

	void VisualScriptNodeInstance::GetPinData(const std::string& aPinName, lua_State* aState)
	{
		for (VisualScriptPinInstance& pin : myPins)
		{
			if (pin.GetName() == aPinName)
			{
				pin.GetPinData(aState);
			}
		}
	}

	void VisualScriptNodeInstance::CallFunction(const char* aName, int aScriptID)
	{
		std::string id = std::to_string(myID);

		myNode->CallFunction(aName, id.c_str(), aScriptID);
	}

	bool VisualScriptNodeInstance::IsStartNode()
	{
		return myNode->IsStartNode();
	}

}