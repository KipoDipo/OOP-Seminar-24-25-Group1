#include <iostream>
#include <fstream>

int main()
{
	std::ifstream inFile("input.txt");
	if (!inFile.is_open())
		return -1;

	std::ofstream outFile("output.txt");
	if (!outFile.is_open())
		return -2;

	char buffer[256];
	while (inFile.getline(buffer, 256))
	{
		int length = strlen(buffer);
		for (int i = length - 1; i >= 0; i--)
		{
			outFile << buffer[i];
		}
		outFile << "\n";
	}
}