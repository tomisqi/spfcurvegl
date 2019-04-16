#include <stdlib.h>
#include "vector.h"
#include "peano.h"
#include "math.h"
#include "curve.h"

struct PeanoSequence
{
	Vector2 steps[8];
	CurveType recurseSeq[9];
};

static PeanoSequence pSeqs[4] =
{
	{ // TYPE_0 ... 5-up
		{VECTOR2_LEFT, VECTOR2_LEFT, VECTOR2_DOWN, VECTOR2_RIGHT, VECTOR2_RIGHT, VECTOR2_DOWN, VECTOR2_LEFT, VECTOR2_LEFT},
		{TYPE_0, TYPE_3, TYPE_0, TYPE_2, TYPE_1, TYPE_2, TYPE_0, TYPE_3, TYPE_0}
	},
	{ // TYPE_1 ... 5-down
		{VECTOR2_RIGHT, VECTOR2_RIGHT, VECTOR2_UP, VECTOR2_LEFT, VECTOR2_LEFT, VECTOR2_UP, VECTOR2_RIGHT, VECTOR2_RIGHT},
		{TYPE_1, TYPE_2, TYPE_1, TYPE_3, TYPE_0, TYPE_3, TYPE_1, TYPE_2, TYPE_1}
	},
	{ // TYPE_2 ... 2-up
		{VECTOR2_RIGHT, VECTOR2_RIGHT, VECTOR2_DOWN, VECTOR2_LEFT, VECTOR2_LEFT, VECTOR2_DOWN, VECTOR2_RIGHT, VECTOR2_RIGHT},
		{TYPE_2, TYPE_1, TYPE_2, TYPE_0, TYPE_3, TYPE_0, TYPE_2, TYPE_1, TYPE_2}
	},
	{ // TYPE_3 ... 2-down
		{VECTOR2_LEFT, VECTOR2_LEFT, VECTOR2_UP, VECTOR2_RIGHT, VECTOR2_RIGHT, VECTOR2_UP, VECTOR2_LEFT, VECTOR2_LEFT},
		{TYPE_3, TYPE_0, TYPE_3, TYPE_1, TYPE_2, TYPE_1, TYPE_3, TYPE_0, TYPE_3}
	},
};

static int GetPeanoPointCount(int level)
{
	int result = pow(9, level); // 3^level * 3^level = 9^level
	return result;
}

static float GetPeanoTileSize(int level, int canvasSize)
{
	float result = canvasSize / pow(3, level);
	return result;
}

static int Peano(Vector2 vectors[], int nextVectorIdx, int level, CurveType type)
{
	int vIdx = nextVectorIdx;
	if (level > 0)
	{
		PeanoSequence pSeq = pSeqs[type];
		for (int i = 0; i < 8; i++)
		{
			vIdx = Peano(vectors, vIdx, level - 1, pSeq.recurseSeq[i]);
			vectors[vIdx++] = pSeq.steps[i];
		}
		vIdx = Peano(vectors, vIdx, level - 1, pSeq.recurseSeq[8]);
	}

	return vIdx;
}

Curve PeanoGenCurve(int level, int canvasSize)
{
	float tileSize = GetPeanoTileSize(level, canvasSize);
	int pointCount = GetPeanoPointCount(level);
	Vector2* vectors = (Vector2*)malloc(pointCount * sizeof(Vector2));

	// Generate the vectors for the peano curve
	Peano(vectors, 1, level, TYPE_2);

	// Transform vectors to points
	Vector2* points = vectors;
	Vector2 startPoint = V2(0.0f, canvasSize) + V2(tileSize / 2.0f, -tileSize / 2.0f); // startPos is the middle of the upper-left tile
	points[0] = startPoint;
	for (int i = 1; i < pointCount; i++)
	{
		points[i] = (tileSize * vectors[i]) + points[i - 1];
	}

	// Create data structure and return
	Curve result = { points, pointCount, tileSize };
	return result;
}