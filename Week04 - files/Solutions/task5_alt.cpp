#include <iostream>
#include <fstream>

struct Order
{
	double grossPrice;
	double vat;
	int customerID;
};

bool saveOrderToFile(const char* fileName, const Order& order)
{
	std::ofstream out(fileName, std::ios::binary | std::ios::app);
	if (!out.is_open())
		return false;

	out.write(reinterpret_cast<const char*>(&order), sizeof(order));

	return true;
}

Order* allOrders(const char* fileName, int& size)
{
	std::ifstream in(fileName, std::ios::binary | std::ios::ate);
	if (!in.is_open())
		return nullptr;

	size = in.tellg() / sizeof(Order);
	in.seekg(std::ios::beg);

	Order* orders = new Order[size];
	in.read(reinterpret_cast<char*>(orders), sizeof(Order) * size);

	return orders;
}

int main()
{
	int orderCount = 0;
	Order* orders = allOrders("storeOrders.bin", orderCount);

	for (int i = 0; i < orderCount; i++)
	{
		std::cout << orders[i].customerID << " "
			<< orders[i].grossPrice << " "
			<< orders[i].vat << "\n";
	}

	delete[] orders;

	return 0;
}