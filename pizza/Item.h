#pragma once

#include <string>
#include <vector>
#include <string_view>

class Item {
public:
	Item(std::string_view name, float price)
		: m_name(name), m_price(price) {}
	Item() : m_name(""), m_price(0) {}

	std::string m_name;
	float m_price;
	std::vector<Item> childItemVector;
	static std::vector<Item> allItems;
	static std::vector<Item> currentItems;
	enum itemState {
		none = 0,
		exists = 1,
		created = 2,
		deleted = 3,
		reEnter = 4
	};
	static itemState optionStat;
	static itemState itemStat;
	bool CheckIfCredentialsExist(Item tObj, std::vector<Item> vecOfItems);
	static void DeserealizeItemsToVec(std::vector<Item>& vecOfItems);
	static void SerealizeItems(std::vector<Item>& vecOfItems);
	bool operator==(Item x);

	template<class Archive>
	friend void serialize(Archive& archive, Item& item);
};

template<class Archive>
void serialize(Archive& archive, Item& item)
{
	archive(item.m_name, item.m_price, item.childItemVector);
}