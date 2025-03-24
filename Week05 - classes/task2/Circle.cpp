#include "Circle.h"
#include <cmath>

Circle::Circle(float radius, Point location)
{
	if (radius <= 0)
		radius = 1;

	this->location = location;
	this->radius = radius;
}

void Circle::setLocation(Point location)
{
	this->location = location;
}

Point Circle::getLocation() const
{
	return location;
}

float Circle::getRadius() const
{
	return radius;
}

bool Circle::contains(Point point) const
{
	return pow(point.x - location.x, 2) + pow(point.y - location.y, 2) <= pow(radius, 2);
}
