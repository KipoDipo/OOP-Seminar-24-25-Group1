﻿#include <iostream>
#include "Triangle.h"

int main()
{
	Triangle t(3, 4, 5);

	std::cout << t.calculatePerimeter() << '\n';
	std::cout << t.calculateArea() << '\n';
	std::cout << t.calculateInscribedRadius() << '\n';
	std::cout << t.calculateCircumscribedRadius() << '\n';
}