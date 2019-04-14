#pragma once
#include "vector.h"

struct HilbertCurve
{
	Vector2* points;
	int pointCount;
	float tileSize;
};

HilbertCurve HilbertGenCurve(int level, int canvasSize);