#include <stdlib.h>
#include "vector.h"
#include "hilbert.h"

static float GetHilbertTileSize(int hilbertLevel, int screenSize)
{
	float result = screenSize / (2 << (hilbertLevel - 1));
	return result;
}

static float GetHilbertPointCount(int hilbertLevel)
{
	int result = (2 << (hilbertLevel - 1)) * (2 << (hilbertLevel - 1));
	return result;
}

static int Hilbert(int level, float tileSize, Vector2 direction, Vector2 points[], int nextPoint)
{
	int p = nextPoint;
	if (level > 0)
	{
		if (direction == VECTOR2_LEFT)
		{
			p = Hilbert(level - 1, tileSize, VECTOR2_UP, points, p);			
			points[p] = points[p - 1] + tileSize * VECTOR2_RIGHT;
			p++;
			p = Hilbert(level - 1, tileSize, VECTOR2_LEFT, points, p);			
			points[p] = points[p - 1] + tileSize * VECTOR2_DOWN;
			p++;			
			p = Hilbert(level - 1, tileSize, VECTOR2_LEFT, points, p);			
			points[p] = points[p - 1] + tileSize * VECTOR2_LEFT;
			p++;
			p = Hilbert(level - 1, tileSize, VECTOR2_DOWN, points, p);
		}
		if (direction == VECTOR2_RIGHT)
		{
			p = Hilbert(level - 1, tileSize, VECTOR2_DOWN, points, p);
			points[p] = points[p - 1] + tileSize * VECTOR2_LEFT;
			p++;
			p = Hilbert(level - 1, tileSize, VECTOR2_RIGHT, points, p);
			points[p] = points[p - 1] + tileSize * VECTOR2_UP;
			p++;
			p = Hilbert(level - 1, tileSize, VECTOR2_RIGHT, points, p);
			points[p] = points[p - 1] + tileSize * VECTOR2_RIGHT;
			p++;
			p = Hilbert(level - 1, tileSize, VECTOR2_UP, points, p);
		}
		if (direction == VECTOR2_UP)
		{
			p = Hilbert(level - 1, tileSize, VECTOR2_LEFT, points, p);
			points[p] = points[p - 1] + tileSize * VECTOR2_DOWN;
		    p++;
			p = Hilbert(level - 1, tileSize, VECTOR2_UP, points, p);
			points[p] = points[p - 1] + tileSize * VECTOR2_RIGHT;
			p++;
			p = Hilbert(level - 1, tileSize, VECTOR2_UP, points, p);
			points[p] = points[p - 1] + tileSize * VECTOR2_UP;
			p++;
			p = Hilbert(level - 1, tileSize, VECTOR2_RIGHT, points, p);
		}
		if (direction == VECTOR2_DOWN)
		{
			p = Hilbert(level - 1, tileSize, VECTOR2_RIGHT, points, p);
			points[p] = points[p - 1] + tileSize * VECTOR2_UP;
			p++;
			p = Hilbert(level - 1, tileSize, VECTOR2_DOWN, points, p);
			points[p] = points[p - 1] + tileSize * VECTOR2_LEFT;
			p++;
			p = Hilbert(level - 1, tileSize, VECTOR2_DOWN, points, p);
			points[p] = points[p - 1] + tileSize * VECTOR2_DOWN;
			p++;
			p = Hilbert(level - 1, tileSize, VECTOR2_LEFT, points, p);
		}
	}

	return p;
}

HilbertCurve HilbertGenCurve(int level, int canvasSize)
{
	float tileSize = GetHilbertTileSize(level, canvasSize);
	int pointCount = GetHilbertPointCount(level);
	Vector2* points = (Vector2*)malloc(pointCount * sizeof(Vector2));
	Vector2 startPos = V2(0.0f, canvasSize) + V2(tileSize / 2.0f, -tileSize / 2.0f); // startPos is the middle of the upper-left tile
	points[0] = startPos;
	Hilbert(level, tileSize, VECTOR2_UP, points, 1);

	HilbertCurve result = {points, pointCount, tileSize};
	return result;
}