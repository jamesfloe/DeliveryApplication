#include "Employee.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <string_view>

std::vector<Employee> Employee::loggedInEmployees;
std::vector<Employee> Employee::allEmployees;

bool Employee::CheckIfCredentialsExist(Employee tObj, std::vector<Employee> vecOfEmployees)
{
    if (vecOfEmployees.size() == 0) {
        return false;
    }
    for (Employee employee : vecOfEmployees) {
        if (tObj == employee) {
            return true;
        }
    }
    return false;
}

bool Employee::operator==(Employee x)
{
    if (m_name == x.m_name && m_password == x.m_password) {
        return true;
    }
    else {
        return false;
    }
}

void Employee::DeserealizeEmployeesToVec(std::vector<Employee>& vecOfEmployees)
{
    if (vecOfEmployees.size() == 0)
        return;
    std::vector<Employee> tempEmployees;
    std::ifstream input{ "record.bin" };
    cereal::BinaryInputArchive archive{ input };
    archive(tempEmployees);
    vecOfEmployees = tempEmployees;
};

void Employee::SerealizeEmployees(std::vector<Employee>& vecOfEmployees)
{
    std::ofstream os("record.bin", std::ios::binary);
    cereal::BinaryOutputArchive archive(os);
    archive(vecOfEmployees);
};
