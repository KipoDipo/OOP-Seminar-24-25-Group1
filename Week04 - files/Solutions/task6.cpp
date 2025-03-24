#include <iostream>
#include <fstream>
#pragma warning (disable: 4996)

struct Customer
{
	char* name;
	int id;
	int age;
};

bool saveCustomer(const char* fileName, const Customer& customer)
{
	std::ofstream out(fileName, std::ios::binary);
	if (!out.is_open())
		return false;

	int length = strlen(customer.name);

	out.write(reinterpret_cast<const char*>(&length), sizeof(length));
	out.write(reinterpret_cast<const char*>(customer.name), sizeof(char) * length);

	out.write(reinterpret_cast<const char*>(&customer.id), sizeof(customer.id));
	out.write(reinterpret_cast<const char*>(&customer.age), sizeof(customer.age));

	return true;
}


Customer loadCustomer(const char* fileName)
{
	std::ifstream in(fileName, std::ios::binary);

	int length;

	in.read(reinterpret_cast<char*>(&length), sizeof(length));
	if (!in.is_open())
		return Customer{ nullptr, -1, -1 };

	Customer result;
	result.name = new char[length + 1] {};
	
	in.read(reinterpret_cast<char*>(result.name), sizeof(char) * length);

	in.read(reinterpret_cast<char*>(&result.id), sizeof(result.id));
	in.read(reinterpret_cast<char*>(&result.age), sizeof(result.age));

	return result;
}

int main()
{
	Customer customer = loadCustomer("customer.bin");
	std::cout
		<< customer.name << "\n"
		<< customer.age << "\n"
		<< customer.id;

	return 0;
}