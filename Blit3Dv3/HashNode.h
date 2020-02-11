#pragma once
#include "CharMap.h"
#include "Coordinates.h"
#include <iostream>

extern CharMap* charMap;

class HashNode
{
public:
	HashNode* next;

	Coords value;

	HashNode(const Coords &coordinates)
	{
		value = coordinates;
		next = NULL;
	}

	~HashNode()
	{
		if (next != NULL)
			delete next;
	}
};

class KeyNode
{
public:
	KeyNode* next;

	HashNode* data;

	char key;

	KeyNode(const char &hashKey, const Coords &value)
	{
		key = hashKey;
		data = new HashNode(value);
		next = NULL;
	}

	~KeyNode()
	{
		if (data != NULL)
			delete data;

		if (next != NULL)
			delete next;
	}
};

class HashMap
{
private:
	KeyNode* head;

	void addHashNode(KeyNode* keyHead, const Coords &value)
	{
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		//Method			:	displayHashMap
		//
		//Method parameters	:	none
		//
		//Method return		:	void
		//
		//Synopsis			:   Adds a new HashNode at the head of a KeyNode's HashList
		//						
		//
		//Modifications		:
		//						Date			Developer				Notes
		//						----			---------				-----
		//						10/01/2018		J. Bordage				Initial Setup
		//																
		//
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

		HashNode* node = new HashNode(value);

		node->next = keyHead->data;

		keyHead->data = node;
	}

public:

	HashMap()
	{
		head = NULL;
	}

	~HashMap()
	{
		if (head != NULL)
			delete head;
	}

	void displayHashMap()
	{
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		//Method			:	displayHashMap
		//
		//Method parameters	:	none
		//
		//Method return		:	void
		//
		//Synopsis			:   Displays the all of the HashNodes' values of every KeyNode in the HashMap
		//						
		//
		//Modifications		:
		//						Date			Developer				Notes
		//						----			---------				-----
		//						10/01/2018		J. Bordage				Initial Setup
		//																
		//
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

		KeyNode* tail = head;
		HashNode* node;
		int counter = 0;

		while (tail != NULL)
		{
			node = tail->data;

			std::cout << "Key: " << tail->key << std::endl;

			while (node != NULL)
			{
				counter++;
				std::cout << "\tPosX: " << node->value.x << ", PosY: " << node->value.y << std::endl;
				node = node->next;
			}

			tail = tail->next;
		}

		std::cout << "There are " << counter << " indices in this HashMap" << std::endl;
	}

	void populateHashMap()
	{
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		//Method			:	populateHashMap
		//
		//Method parameters	:	none
		//
		//Method return		:	void
		//
		//Synopsis			:   Populates the HashMap with values from the CharMap
		//						
		//
		//Modifications		:
		//						Date			Developer				Notes
		//						----			---------				-----
		//						10/01/2018		J. Bordage				Initial Setup
		//																
		//
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

		CharNode* node;

		for (int x = 0; x < charMap->getWidth(); x++)
		{
			node = charMap->getNode(x, 0);

			for (int y = 0; y < charMap->getHeight(); y++)
			{
				addHashData(charMap->getData(node), Coords(x, y));

				if (node->next != NULL)
					node = node->next;
			}
		}
	}

	void addHashData(const char &key, const Coords &value)
	{
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		//Method			:	addHashData
		//
		//Method parameters	:	const char &key, const Coords &value
		//
		//Method return		:	void
		//
		//Synopsis			:   Adds a HashNode of a given key and value to the HashMap
		//						
		//
		//Modifications		:
		//						Date			Developer				Notes
		//						----			---------				-----
		//						10/01/2018		J. Bordage				Initial Setup
		//																
		//
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

		if (head != NULL)
		{
			KeyNode* tail = head;
			bool atEnd = false;

			//Checks if the new key is
			//smaller than the first key
			if (key < head->key)
			{
				//If so, inserts the new key at the start
				KeyNode* node = new KeyNode(key, value);
				node->next = head;
				head = node;
			}
			else
			{
				//Searches the map to see for the new key
				while (!atEnd)
				{
					if (tail->key == key)
					{
						//Inserts a hash node with the new data at the start of the hashkey list
						addHashNode(tail, value);

						//Ends the search
						atEnd = true;
					}
					else
					{
						//Checks if the end of the list has been reached
						if (tail->next != NULL)
						{
							if (tail->next->key <= key)
							{
								//If the next key value is smaller or equal to
								//the new key, the next cycle will handle it
								tail = tail->next;
							}
							else
							{
								//Checks if the next key is larger than the new key value
								//Indicating that the node should be inserted here
								KeyNode* node = new KeyNode(key, value);
								node->next = tail->next;
								tail->next = node;
								atEnd = true;
							}
						}
						else
						{
							//If the key is not already present in the map, make a new keyNode.
							tail->next = new KeyNode(key, value);
							atEnd = true;
						}
					}
				}
			}
		}
		else
		{
			//Assigns a value to the head node
			head = new KeyNode(key, value);
		}
	}

	HashNode* getKeyHash(const char &key)
	{
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		//Method			:	getKeyHash
		//
		//Method parameters	:	const char &key
		//
		//Method return		:	void
		//
		//Synopsis			:   Returns the first HashNode (Head) of a given Hash Key, if it exists
		//						
		//
		//Modifications		:
		//						Date			Developer				Notes
		//						----			---------				-----
		//						10/01/2018		J. Bordage				Initial Setup
		//																
		//
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

		KeyNode* node = head;

		while (node != NULL)
		{
			if (node->key == key) return node->data;
			else node = node->next;
		}

		return NULL;
	}

	Coords getHashValue(HashNode* node)
	{
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		//Method			:	getHashValue
		//
		//Method parameters	:	HashNode* node
		//
		//Method return		:	Coords
		//
		//Synopsis			:   Returns the value of the HashNode
		//						
		//
		//Modifications		:
		//						Date			Developer				Notes
		//						----			---------				-----
		//						10/01/2018		J. Bordage				Initial Setup
		//																
		//
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

		return node->value;
	}
};