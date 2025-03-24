#pragma once

struct Point
{
	float x, y;
};

class Circle
{
public:
	Circle(float radius, Point location);

	void setLocation(Point location);

	Point getLocation() const;
	float getRadius() const;

	bool contains(Point point) const;

private:
	float radius;
	Point location;
};