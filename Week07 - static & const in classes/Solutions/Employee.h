#pragma once

class Company; // forward declaration

class Employee
{
public:
	Employee(const char* first, const char* last, double salary, Employee* boss = nullptr, Company* company = nullptr);
	~Employee();

	Employee() = delete;
	Employee(const Employee& other) = delete;
	Employee& operator=(const Employee& other) = delete;

	void setSalary(double salary);
	void setBoss(Employee* boss);
	void setCompany(Company* company);

	int getId() const;
	const char* getFirstName() const;
	const char* getLastName() const;
	double getSalary() const;

	const Employee* getBoss() const;
	const Company* getCompany() const;

private:
	void free();
	void setName(char*& destination, const char* source);

	static int counter;

	const int id;
	char* first = nullptr;
	char* last = nullptr;
	double salary;

	Employee* boss;
	Company* company;
};