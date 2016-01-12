#include "ScriptPrecompiled.h"
#include "VisualScript.h"

#include "VisualScriptManager.h"
#include "VisualScriptingPinConnection.h"

#include "LuaArgument.h"
#include "LuaArguments.h"

#include "tinyxml2.h"

namespace Script
{
	VisualScript::VisualScript()
	{
	}

	VisualScript::~VisualScript()
	{
	}

	void VisualScript::SetID(int aID)
	{
		myScriptID = aID;
	}

	void VisualScript::Load(const std::string& aFile, VisualScriptManager& aManager)
	{
		tinyxml2::XMLDocument file;
		file.LoadFile(aFile.c_str());

		tinyxml2::XMLElement* node = file.RootElement()->FirstChildElement("Node");

		while (node != nullptr)
		{
			VisualScriptNodeInstance newNode;
			myNodes.push_back(newNode);

			node = node->NextSiblingElement("Node");
		}

		node = file.RootElement()->FirstChildElement("Node");

		int index = 0;
		while (node != nullptr)
		{
			myNodes[index].Load(node, aManager);
			index++;
			
			node = node->NextSiblingElement("Node");
		} // All nodes in script created

		node = file.RootElement()->FirstChildElement("Node"); // Reset variable

		while (node != nullptr) // Time to load all connections between nodes
		{
			std::string uid = node->FirstChildElement("UID")->GetText();

			long long id = std::stoll(uid);

			VisualScriptNodeInstance& startNode = GetNodeFromID(id);

			tinyxml2::XMLElement* connection = node->FirstChildElement("Connections")->FirstChildElement("Connection");
			
			while (connection != nullptr)
			{
				LoadConnection(startNode, connection);

				connection = connection->NextSiblingElement("Connection");
			}

			node = node->NextSiblingElement("Node");
		}
	}

	void VisualScript::ExecutePin(const std::string& aPinName, long long aNodeID)
	{
		GetNodeFromID(aNodeID).ExecutePin(aPinName, myScriptID);
	}

	VisualScriptNodeInstance& VisualScript::GetNodeFromID(long long aID)
	{
		for (VisualScriptNodeInstance& node : myNodes)
		{
			if (node.GetUID() == aID)
			{
				return node;
			}
		}

		return myNodes[0];
	}

	void VisualScript::LoadConnection(VisualScriptNodeInstance& aStartNode, tinyxml2::XMLElement* aConnection)
	{
		std::string type = aConnection->FirstChildElement("Type")->GetText();

		if (type == "Output")
		{
			if (aConnection->FirstChildElement("PinConnection") != nullptr)
			{
				LoadOutputConnection(aStartNode, aConnection);
			}
		}
		else if (type == "Input")
		{
			LoadInputConnection(aStartNode, aConnection);
		}
	}

	void VisualScript::LoadInputConnection(VisualScriptNodeInstance& aStartNode, tinyxml2::XMLElement* aConnection)
	{
		std::string type = aConnection->FirstChildElement("VarType")->GetText();

		if (type == "impulse")
		{
			return;
		}
		else
		{
			int index;

			aConnection->FirstChildElement("Index")->QueryIntText(&index);
			VisualScriptPinInstance& pin = aStartNode.GetPinByIndex(index);

			switch (pin.GetPinType())
			{
			case eVisualScriptVariableTypes::STRING:
			{
				VisualScriptVariable var;
				var.SetVariable(std::string(aConnection->FirstChildElement("PinData")->GetText()));

				pin.SetVariable(var);
			}
				break;
			case eVisualScriptVariableTypes::IMPULSE:
				assert(false && "something is really wrong");
				break;
			case eVisualScriptVariableTypes::INT:
				assert(false && "something is really wrong");
				break;
			case eVisualScriptVariableTypes::FLOAT:
				assert(false && "something is really wrong");
				break;
			case eVisualScriptVariableTypes::BOOL:
				assert(false && "something is really wrong");
				break;
			default:
				break;
			}
		}
	}

	void VisualScript::LoadOutputConnection(VisualScriptNodeInstance& aStartNode, tinyxml2::XMLElement* aConnection)
	{
		int outPinIndex;

		aConnection->FirstChildElement("Index")->QueryIntText(&outPinIndex);

		VisualScriptingPinConnection pinConnection;

		std::string uid = aConnection->FirstChildElement("PinConnection")->FirstChildElement("ConnectedTo")->GetText();
		long long id = std::stoll(uid);

		int inPinIndex;

		aConnection->FirstChildElement("PinConnection")->FirstChildElement("PinIndex")->QueryIntText(&inPinIndex);
		pinConnection.myTargetPin = &GetNodeFromID(id).GetPinByIndex(inPinIndex);

		aStartNode.GetPinByIndex(outPinIndex).SetConnection(pinConnection);
	}

	void VisualScript::CallFunction(const char* aName, int aNumberOfArgs, int /*aNumberOfReturns*/, const LuaArguments& someArguments)
	{
		if (aNumberOfArgs != 1)
		{
			assert(false && "The number of arguments sent to the visual script isn't 1");
		}

		VisualScriptNodeInstance& activeNode = GetNodeFromID(std::stoll(someArguments.GetArgumentFromIndex(0).GetAsString()));

		activeNode.CallFunction(aName, myScriptID);
	}

	int VisualScript::GetID()
	{
		return myScriptID;
	}

	void VisualScript::Start()
	{
		for (VisualScriptNodeInstance& node : myNodes)
		{
			if (node.IsStartNode() == true)
			{
				node.Run(myScriptID);
				return;
			}
		}
	}

	void VisualScript::GetPinData(const std::string& aPinName, long long aCallerID, lua_State* aState)
	{
		GetNodeFromID(aCallerID).GetPinData(aPinName, aState);
	}

}