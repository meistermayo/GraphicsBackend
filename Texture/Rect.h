#ifndef RECT_H
#define RECT_H

struct Rect
{
public:
	Rect(float x, float y, float width, float height)
		: x(x), y(y), width(width), height(height) {}
	Rect()
		: x(0.f), y(0.f), width(0.f), height(0.f) {}

	void clear()
	{
		x = 0.0f;
		y = 0.0f;
		width = 0.0f;
		height = 0.0f;
	}

	float x;
	float y;
	float width;
	float height;
};

#endif