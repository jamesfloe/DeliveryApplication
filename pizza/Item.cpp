#include "Employee.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <string_view>

std::vector<Item> Item::allItems;
std::vector<Item> Item::currentItems;

bool Item::CheckIfCredentialsExist(Item tObj, std::vector<Item> vecOfItems)
{
    if (vecOfItems.size() == 0) {
        return false;
    }
    for (Item item : vecOfItems) {
        if (tObj == item) {
            return true;
        }
    }
    return false;
}

bool Item::operator==(Item x)
{
    if (m_name == x.m_name && m_price == x.m_price) {
        return true;
    }
    else {
        return false;
    }
}

void Item::DeserealizeItemsToVec(std::vector<Item>& vecOfItems)
{
    std::vector<Item> tempItems;
    std::ifstream input{ "record.bin" };
    cereal::BinaryInputArchive archive{ input };
    archive(tempItems);
    vecOfItems = tempItems;
};

void Item::SerealizeItems(std::vector<Item>& vecOfItems)
{
    std::ofstream os("record.bin", std::ios::binary);
    cereal::BinaryOutputArchive archive(os);
    archive(vecOfItems);
};