#include "Triangle.h"
#include <cmath>

Triangle::Triangle(float a, float b, float c)
	: a(a), b(b), c(c)
{
}

void Triangle::setA(float a)
{
	this->a = a;
}

void Triangle::setB(float b)
{
	this->b = b;
}

void Triangle::setC(float c)
{
	this->c = c;
}

float Triangle::getA() const
{
	return a;
}

float Triangle::getB() const
{
	return b;
}

float Triangle::getC() const
{
	return c;
}

float Triangle::calculateArea() const
{
	float s = calculatePerimeter() / 2.0f;

	return sqrt(s * (s - a) * (s - b) * (s - c));
}

float Triangle::calculatePerimeter() const
{
	return a + b + c;
}

float Triangle::calculateInscribedRadius() const
{
	float s = calculatePerimeter() / 2.0f;

	return calculateArea() / s;
}

float Triangle::calculateCircumscribedRadius() const
{
	return (a * b * c) / (4 * calculateArea());
}
