#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <iostream>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <string_view>
#include <ctime>
#include "Delivery.h"

class Employee {
public:
	Employee(std::string_view name, std::string_view password, std::time_t loginTime)
		: m_name{ name }, m_password{ password }, loginTime{ loginTime } {}
	Employee() = default;
	~Employee() = default;

	std::time_t loginTime;
	std::string m_name;
	std::vector<Delivery> AssignedDeliveries;
	static std::vector<Employee> loggedInEmployees;
	static std::vector<Employee> allEmployees;
	enum employeeState {
		none = 0,
		exists = 1,
		created = 2,
		deleted = 3,
		reEnter = 4
	};
	static employeeState empStat;
	bool CheckIfCredentialsExist(Employee tObj, std::vector<Employee> vecOfEmployees);
	static void DeserealizeEmployeesToVec(std::vector<Employee>& vecOfEmployees);
	static void SerealizeEmployees(std::vector<Employee>& vecOfEmployees);
	bool operator==(Employee x);

	template<class Archive>
	friend void serialize(Archive& archive, Employee& employee);

private:
	std::string m_password;
};

template<class Archive>
void serialize(Archive& archive, Employee& employee)
{
	archive(employee.m_name, employee.m_password);
}
