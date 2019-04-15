#include <stdlib.h>
#include "vector.h"
#include "hilbert.h"

enum HilbertType
{
	TYPE_0 = 0, // up
	TYPE_1,		// left
	TYPE_2,		// down
	TYPE_3,		// right
};

struct HilbertSequence
{
	Vector2 steps[3];
	HilbertType typeSeq[4];
};

static HilbertSequence hSeqs[4] =
{
	{ // TYPE_0
		{VECTOR2_DOWN, VECTOR2_RIGHT, VECTOR2_UP},
		{TYPE_1, TYPE_0, TYPE_0, TYPE_3}
	},
	{ // TYPE_1
		{VECTOR2_RIGHT, VECTOR2_DOWN, VECTOR2_LEFT},
		{TYPE_0, TYPE_1, TYPE_1, TYPE_2}
	},
	{ // TYPE_2
		{VECTOR2_UP, VECTOR2_LEFT, VECTOR2_DOWN},
		{TYPE_3, TYPE_2, TYPE_2, TYPE_1}
	},
	{ // TYPE_3
		{VECTOR2_LEFT, VECTOR2_UP, VECTOR2_RIGHT},
		{TYPE_2, TYPE_3, TYPE_3, TYPE_0}
	},
};

static float GetHilbertPointCount(int level)
{
	int result = (2 << (level - 1)) * (2 << (level - 1));
	return result;
}

static float GetHilbertTileSize(int level, int canvasSize)
{
	float result = canvasSize/ (2 << (level - 1));
	return result;
}

static int Hilbert(Vector2 vectors[], int nextVectorIdx, int level, HilbertType type)
{
	int vIdx = nextVectorIdx;
	if (level > 0)
	{
		HilbertSequence hSeq = hSeqs[type];

		vIdx = Hilbert(vectors, vIdx, level - 1, hSeq.typeSeq[0]);
		vectors[vIdx++] = hSeq.steps[0];
		vIdx = Hilbert(vectors, vIdx, level - 1, hSeq.typeSeq[1]);
		vectors[vIdx++] = hSeq.steps[1];
		vIdx = Hilbert(vectors, vIdx, level - 1, hSeq.typeSeq[2]);
		vectors[vIdx++] = hSeq.steps[2];
		vIdx = Hilbert(vectors, vIdx, level - 1, hSeq.typeSeq[3]);
	}

	return vIdx;
}

HilbertCurve HilbertGenCurve(int level, int canvasSize)
{
	float tileSize = GetHilbertTileSize(level, canvasSize);
	int pointCount = GetHilbertPointCount(level);
	Vector2* vectors = (Vector2*)malloc(pointCount * sizeof(Vector2));
	Hilbert(vectors, 1, level, TYPE_0);

	// Hilbert() produces vectors, but we need actual points in the canvas
	Vector2* points = vectors;
	Vector2 startPoint = V2(0.0f, canvasSize) + V2(tileSize / 2.0f, -tileSize / 2.0f); // startPos is the middle of the upper-left tile
	points[0] = startPoint;
	for (int i = 1; i < pointCount; i++)
	{
		points[i] = (tileSize * vectors[i]) + points[i - 1];
	}

	HilbertCurve result = {points, pointCount, tileSize};
	return result;
}