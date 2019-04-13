#pragma once

struct Color
{
	float r;
	float b;
	float g;
	float a;
};

#define COLOR_WHITE NewColor(1.0f, 1.0f, 1.0f)
#define COLOR_BLACK NewColor(0.0f, 0.0f, 0.0f)
#define COLOR_RED   NewColor(1.0f, 0.0f, 0.0f)
static inline Color NewColor(float r, float g, float b)
{
	Color result;
	result.r = r;
	result.g = g;
	result.b = b;
	result.a = 1.0f;

	return result;
}