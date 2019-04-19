#pragma once

enum CurveType
{
	HILBERT,
	PEANO,
};

struct CurveDrawData
{
	Vector2* points;
	int pointCount;
	float tileSize;
};

CurveDrawData GenCurve(CurveType curveType, int level, int canvasSize);