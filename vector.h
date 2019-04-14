#pragma once

struct Vector2
{
	float x;
	float y;
};

#define VECTOR2_ZERO	V2(0.0f, 0.0f)
#define VECTOR2_ONE		V2(1.0f, 1.0f)
#define VECTOR2_RIGHT	V2(1.0f, 0.0f)
#define VECTOR2_UP		V2(0.0f, 1.0f)
#define VECTOR2_LEFT	V2(-1.0f, 0.0f)
#define VECTOR2_DOWN	V2(0.0f, -1.0f)
static inline Vector2 V2(float x, float y)
{
	Vector2 result = {x, y};
	return result;
}

static inline Vector2 operator+(Vector2 a, Vector2 b)
{
	Vector2 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;

	return result;
}

static inline Vector2 operator-(Vector2 a, Vector2 b)
{
	Vector2 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;

	return result;
}

static inline Vector2 operator*(float s, Vector2 a)
{
	Vector2 result;
	result.x = s * a.x;
	result.y = s * a.y;

	return result;
}

static inline Vector2 & operator+=(Vector2 &a, Vector2 b)
{
	a = a + b;
	return a;
}

static inline Vector2 & operator-=(Vector2 &a, Vector2 b)
{
	a = a - b;
	return a;
}

static inline bool operator==(Vector2 a, Vector2 b)
{
	return (a.x == b.x) && (a.y == b.y);
}