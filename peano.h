#pragma once
#include "vector.h"

struct PeanoCurve
{
	Vector2* points;
	int pointCount;
	float tileSize;
};

PeanoCurve PeanoGenCurve(int level, int canvasSize);