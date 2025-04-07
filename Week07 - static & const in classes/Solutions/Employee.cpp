#include "Employee.h"
#include "Company.h"
#include <iostream>
#include <exception>

#pragma warning(disable : 4996)

int Employee::counter = 0;

Employee::Employee(const char* first, const char* last, double salary, Employee* boss, Company* company)
	: id(counter++), boss(boss), company(company)
{
	try
	{
		setName(this->first, first);
		setName(this->last, last);
		setSalary(salary);
	}
	catch (const std::exception& e) // all exceptions with a form of std::exception
	{
		std::cerr << "Error: " << e.what() << std::endl;
		free();
		throw;
	}
	catch (...) // any other exceptions (useless in our case)
	{
		free();
		throw;
	}
	/*
		Alternatively:
		assume the setters are "bool" and use "std::nothrow" when allocating

		if (!setName(first, this->first) ||
			!setName(last, this->last) ||
			!setSalary(salary))
		{
			free();
			throw "Bad input data";
		}
	*/
}

Employee::~Employee()
{
	free();
}

void Employee::setSalary(double salary)
{
	if (salary < 0)
		throw std::exception("negative salary");

	this->salary = salary;
}

void Employee::setBoss(Employee* boss)
{
	this->boss = boss;
}

void Employee::setCompany(Company* company)
{
	this->company = company;
}

int Employee::getId() const
{
	return id;
}

const char* Employee::getFirstName() const
{
	return first;
}

const char* Employee::getLastName() const
{
	return last;
}

double Employee::getSalary() const
{
	return salary;
}

const Employee* Employee::getBoss() const
{
	return boss;
}

const Company* Employee::getCompany() const
{
	return company;
}

void Employee::free()
{
	delete[] first;
	delete[] last;
}




void Employee::setName(char*& destination, const char* source)
{
	if (!source)
		throw std::exception("nullptr not supported");

	int newLength = strlen(source);
	char* newData = new char[newLength + 1];

	strcpy(newData, source);
	newData[newLength] = '\0';

	delete[] destination;
	destination = newData;
}
