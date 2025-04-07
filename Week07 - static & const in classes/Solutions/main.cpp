#include <iostream>
#include "Employee.h"
#include "Company.h"

int main()
{
	Company company("My Company", 5);
	company.setBoss("John", "Doe", 50000);
	try
	{
		company.addEmployee("Jane", "Smith", 40000);
		company.addEmployee("Alice", "Johnson", 45000);
		company.addEmployee("Bob", "Brown", 42000);
		company.addEmployee("Charlie", "Davis", 48000);
		company.addEmployee("Eve", "Wilson", 46000);
		company.addEmployee("Frank", "Garcia", 47000); // This should not be added, as maxEmployees is 5
	}
	catch (const std::exception& e)
	{
		std::cerr << "=============== [Error]: " << e.what() << std::endl;
	}
	
	std::cout << "Company Name: " << company.getName() << std::endl;
	std::cout << "Boss: " << company.getBoss()->getFirstName() << " " << company.getBoss()->getLastName() << std::endl;

	std::cout << "Employees:" << std::endl;
	for (unsigned i = 0; i < company.getEmployeeCount(); ++i)
	{
		Employee* emp = company.getEmployee(i);
		std::cout << emp->getFirstName() << " " << emp->getLastName() << " #" << emp->getId() << ", Salary: " << emp->getSalary()
			<< " working for " << emp->getCompany()->getName()
			<< " under the command of " << emp->getBoss()->getFirstName()[0] << ". " << emp->getBoss()->getLastName() << std::endl;
	}
}