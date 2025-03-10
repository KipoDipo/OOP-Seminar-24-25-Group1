#include <iostream>
#include <fstream>

int* readNums(const char* fileName, int& N)
{
	N = 0;

	std::ifstream inFile(fileName);
	if (!inFile.is_open())
	{
		return nullptr;
	}

	inFile >> N;

	int* arr = new int[N];

	for (int i = 0; i < N; i++)
	{
		inFile >> arr[i];
	}

	return arr;
}

int main()
{
	int N = 0;
	int* arr = readNums("numabers.txt", N);
	if (arr == nullptr)
	{
		std::cout << "Couldn't open file";
		return -1;
	}

	for (int i = 0; i < N; i++)
	{
		std::cout << arr[i] << " ";
	}

	delete[] arr;
}