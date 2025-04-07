#pragma once

class Employee; // forward declaration

class Company
{
public:
	Company(const char* name, unsigned maxEmployees);
	~Company();

	Company() = delete;
	Company(const Company& other) = delete;
	Company& operator=(const Company& other) = delete;

	
	void setBoss(const char* firstName, const char* lastName, double salary);
	void addEmployee(const char* firstName, const char* lastName, double salary);
		

	const char* getName() const;
	Employee* getEmployee(unsigned index) const;
	Employee* getBoss() const;
	unsigned getMaxEmployees() const;
	unsigned getEmployeeCount() const;

private:
	void setName(const char* name);
	void allocateEmployees(unsigned maxEmployees);

	void free();

	char* name = nullptr;

	Employee* boss = nullptr;
	Employee** employees = nullptr;
	unsigned maxEmployees;
	unsigned employeeCount = 0;
};