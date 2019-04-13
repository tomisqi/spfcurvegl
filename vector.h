#pragma once

struct Vector2
{
	float x;
	float y;
};

static inline Vector2 V2(float x, float y)
{
	Vector2 result;
	result.x = x;
	result.y = y;

	return result;
}