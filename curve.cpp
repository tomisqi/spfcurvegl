#include <stdlib.h>
#include "vector.h"
#include "curve.h"
#include "math.h"

#define MAX_SEQ 9

struct CurveSequence
{
	Vector2 steps[MAX_SEQ - 1];
	int recursiveSeq[MAX_SEQ];
};

struct CurveData
{
	CurveSequence* seq;
	int stepCount;
};

static CurveSequence hilbertSeq[4] =
{
	{ // 0 ... v
		{VECTOR2_DOWN, VECTOR2_RIGHT, VECTOR2_UP},
		{1, 0, 0, 3}
	},
	{ // 1 ... >
		{VECTOR2_RIGHT, VECTOR2_DOWN, VECTOR2_LEFT},
		{0, 1, 1, 2}
	},
	{ // 2 ... ^
		{VECTOR2_UP, VECTOR2_LEFT, VECTOR2_DOWN},
		{3, 2, 2, 1}
	},
	{ // 3 ... <
		{VECTOR2_LEFT, VECTOR2_UP, VECTOR2_RIGHT},
		{2, 3, 3, 0}
	},
};

static CurveSequence peanoSeq[4] =
{
	{ // 0 ... 2-up
		{VECTOR2_RIGHT, VECTOR2_RIGHT, VECTOR2_DOWN, VECTOR2_LEFT, VECTOR2_LEFT, VECTOR2_DOWN, VECTOR2_RIGHT, VECTOR2_RIGHT},
		{0, 1, 0, 2, 3, 2, 0, 1, 0}
	},
	{ // 1 ... 5-down
		{VECTOR2_RIGHT, VECTOR2_RIGHT, VECTOR2_UP, VECTOR2_LEFT, VECTOR2_LEFT, VECTOR2_UP, VECTOR2_RIGHT, VECTOR2_RIGHT},
		{1, 0, 1, 3, 2, 3, 1, 0, 1}
	},
	{ // 2 ... 5-up
		{VECTOR2_LEFT, VECTOR2_LEFT, VECTOR2_DOWN, VECTOR2_RIGHT, VECTOR2_RIGHT, VECTOR2_DOWN, VECTOR2_LEFT, VECTOR2_LEFT},
		{2, 3, 2, 0, 1, 0, 2, 3, 2}
	},
	{ // 3 ... 2-down
		{VECTOR2_LEFT, VECTOR2_LEFT, VECTOR2_UP, VECTOR2_RIGHT, VECTOR2_RIGHT, VECTOR2_UP, VECTOR2_LEFT, VECTOR2_LEFT},
		{3, 2, 3, 1, 0, 1, 3, 2, 3}
	},
};

// Curve defintions
CurveData hilbertData = { hilbertSeq, 3 };
CurveData peanoData = { peanoSeq, 8 };

static int CalcHilbertPointCount(int level)
{
	int result = pow(4, level);  // 2^level * 2^level = 4^level
	return result;
}

static float CalcHilbertTileSize(int level, int canvasSize)
{
	float result = canvasSize / pow(2, level);
	return result;
}

static int CalcPeanoPointCount(int level)
{
	int result = pow(9, level); // 3^level * 3^level = 9^level
	return result;
}

static float CalcPeanoTileSize(int level, int canvasSize)
{
	float result = canvasSize / pow(3, level);
	return result;
}

static int GenCurveVectors(CurveData* curve, Vector2 vectors[], int nextVectorIdx, int level, int type)
{
	int vIdx = nextVectorIdx;
	if (level > 0)
	{
		CurveSequence seq = curve->seq[type];
		for (int i = 0; i < curve->stepCount; i++)
		{
			vIdx = GenCurveVectors(curve, vectors, vIdx, level - 1, seq.recursiveSeq[i]);
			vectors[vIdx++] = seq.steps[i];
		}
		vIdx = GenCurveVectors(curve, vectors, vIdx, level - 1, seq.recursiveSeq[curve->stepCount]);
	}

	return vIdx;
}

CurveDrawData GenCurve(CurveType curveType, int level, int canvasSize)
{
	float tileSize;
	int pointCount = 0;
	CurveData* curveData;
	if (curveType == HILBERT)
	{
		tileSize = CalcHilbertTileSize(level, canvasSize);
		pointCount = CalcHilbertPointCount(level);
		curveData = &hilbertData;
	}
	else if (curveType == PEANO)
	{
		tileSize = CalcPeanoTileSize(level, canvasSize);
		pointCount = CalcPeanoPointCount(level);
		curveData = &peanoData;
	}

	Vector2* vectors = (Vector2*)malloc(pointCount * sizeof(Vector2));

	// Generate the vectors for the curve
	GenCurveVectors(curveData, vectors, 1, level, 0);

	// Transform vectors to points
	Vector2* points = vectors; // Note: Vectors will be overwritten
	Vector2 startPoint = V2(0.0f, canvasSize) + V2(tileSize / 2.0f, -tileSize / 2.0f); // startPos is the middle of the upper-left tile
	points[0] = startPoint;
	for (int i = 1; i < pointCount; i++)
	{
		points[i] = (tileSize * vectors[i]) + points[i - 1];
	}

	// Create data structure and return
	CurveDrawData result = {points, pointCount, tileSize};
	return result;
}