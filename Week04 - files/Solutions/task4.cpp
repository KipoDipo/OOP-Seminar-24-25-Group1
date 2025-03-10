#include <iostream>
#include <fstream>

int main()
{
	int n;

	std::ifstream inFile("numbers.bin", std::ios::binary);
	if (!inFile.is_open())
		return -1;

	inFile.read(reinterpret_cast<char*>(&n), sizeof(n));

	int max = INT_MIN;

	for (int i = 0; i < n; i++)
	{
		int number;
		inFile.read(reinterpret_cast<char*>(&number), sizeof(number));

		if (number > max)
			max = number;
	}

	std::cout << max;
}