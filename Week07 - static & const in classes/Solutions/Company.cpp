#include "Company.h"
#include "Employee.h"
#include <exception>
#include <iostream>

#pragma warning(disable : 4996)

Company::Company(const char* name, unsigned maxEmployees)
	: name(nullptr), employees(nullptr), maxEmployees(0)
{
	try
	{
		setName(name);
		allocateEmployees(maxEmployees);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		throw;
	}
	catch (...) // any other exceptions (useless in our case)
	{
		throw;
	}
}

Company::~Company()
{
	free();
}



void Company::setName(const char* name)
{
	if (!name)
		throw std::exception("nullptr is not valid");

	char* newName = new char[strlen(name) + 1];
	strcpy(newName, name);

	delete[] this->name;
	this->name = newName;
}

void Company::allocateEmployees(unsigned maxEmployees)
{
	if (maxEmployees == 0)
		throw std::exception("maxEmployees is 0");

	Employee** newEmployees = new Employee * [maxEmployees];

	for (unsigned i = 0; i < maxEmployees; ++i)
	{
		newEmployees[i] = nullptr;
	}

	// alternatively:
	// Employee** newEmployees = new Employee * [maxEmployees] {}; // initialize all to nullptr

	employees = newEmployees;
	this->maxEmployees = maxEmployees;
}

void Company::free()
{
	delete boss;
	for (unsigned i = 0; i < maxEmployees; i++)
		delete employees[i];

	delete[] employees;
	delete[] name;
}

void Company::setBoss(const char* firstName, const char* lastName, double salary)
{
	Employee* newBoss = new Employee(firstName, lastName, salary);
	newBoss->setBoss(newBoss);
	newBoss->setCompany(this);

	if (boss)
		delete boss;
	boss = newBoss;

	for (unsigned i = 0; i < employeeCount; i++)
	{
		employees[i]->setBoss(boss);
	}
}

Employee* Company::getBoss() const
{
	return boss;
}

const char* Company::getName() const
{
	return name;
}

Employee* Company::getEmployee(unsigned index) const
{
	return employees[index];
}

void Company::addEmployee(const char* firstName, const char* lastName, double salary)
{
	if (employeeCount >= maxEmployees)
		throw std::exception("maxEmployees is reached");

	Employee* newEmployee = new Employee(firstName, lastName, salary);
	newEmployee->setCompany(this);
	newEmployee->setBoss(boss);
	
	employees[employeeCount] = newEmployee;
	employeeCount++;
}

unsigned Company::getMaxEmployees() const
{
	return maxEmployees;
}

unsigned Company::getEmployeeCount() const
{
	return employeeCount;
}
