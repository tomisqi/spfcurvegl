#pragma once

struct Color
{
	float r;
	float g;
	float b;
	float a;
};

#define COLOR_WHITE Col(1.0f, 1.0f, 1.0f)
#define COLOR_BLACK Col(0.0f, 0.0f, 0.0f)
#define COLOR_RED   Col(1.0f, 0.0f, 0.0f)
#define COLOR_BLUE  Col(0.0f, 0.0f, 1.0f)
#define COLOR_GREEN Col(0.0f, 1.0f, 0.0f)
static inline Color Col(float r, float g, float b, float a = 1.0f)
{
	Color result = {r, g, b, a};
	return result;
}