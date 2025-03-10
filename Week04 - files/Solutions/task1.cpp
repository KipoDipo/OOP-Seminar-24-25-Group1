#include <iostream>
#include <fstream>

int main()
{
	std::ifstream inFile("main.cpp");
	if (!inFile.is_open())
		return -1;

	char buffer[256];
	while (inFile.getline(buffer, 256))
	{
		std::cout << buffer << "\n";
	}
}