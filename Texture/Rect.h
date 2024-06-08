#ifndef RECT_H
#define RECT_H

struct Rect
{
public:
	// Constructors
	Rect(float x, float y, float width, float height)
		: x(x), y(y), width(width), height(height) {}
	Rect()
		: x(0.f), y(0.f), width(0.f), height(0.f) {}

	// Clear method
	void clear()
	{
		x = 0.0f;
		y = 0.0f;
		width = 0.0f;
		height = 0.0f;
	}

	// Structure data, public
	float x;
	float y;
	float width;
	float height;
};

#endif