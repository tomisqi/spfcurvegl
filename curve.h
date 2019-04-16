#pragma once
#include "vector.h"

enum CurveType
{
	TYPE_0 = 0,
	TYPE_1,
	TYPE_2,
	TYPE_3,
};

struct Curve
{
	Vector2* points;
	int pointCount;
	float tileSize;
};