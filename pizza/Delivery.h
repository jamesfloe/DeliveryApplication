#pragma once

#include <string>
#include <vector>
#include <string>
#include <memory>
#include "Item.h"

class Delivery {
public:
	Delivery() : m_address(""), m_price(0) {}
	~Delivery() = default;
	Delivery(std::string_view address, float price)
		: m_address(address), m_price(price) {}
	Delivery(const Delivery& other) : m_address(other.m_address), m_price(other.m_price) {}
	std::string m_address;
	float m_price;
	static std::vector<Delivery> deliveryList;
};
