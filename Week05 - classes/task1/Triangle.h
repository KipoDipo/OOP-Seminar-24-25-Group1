#pragma once

class Triangle
{
public:
	Triangle(float a, float b, float c);

	void setA(float a);
	void setB(float b);
	void setC(float c);

	float getA() const;
	float getB() const;
	float getC() const;

	float calculateArea() const;
	float calculatePerimeter() const;
	float calculateSmallR() const;
	float calculateBigR() const;

private:
	float a, b, c;
};