#include <iostream>
#include "Circle.h"

int main()
{
	Circle circle(2, Point{ 5, 5 });

	std::cout << std::boolalpha << circle.contains(Point{ 6, 8 }) << '\n';
	std::cout << circle.contains(Point{ 5, 6 });
}