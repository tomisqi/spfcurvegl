#include <stdlib.h>
#include "vector.h"
#include "hilbert.h"

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

static int Hilbert(int level, float tileSize, Vector2 direction, Vector2 points[], int nextPoint)
{
	int p = nextPoint;
	if (level > 0)
	{
		Vector2 sublevel[4];
		Vector2 connect[3];
		if (direction == VECTOR2_LEFT)
		{
			connect[0] = VECTOR2_RIGHT;
			connect[1] = VECTOR2_DOWN;
			connect[2] = VECTOR2_LEFT;
			sublevel[0] = VECTOR2_UP;
			sublevel[1] = VECTOR2_LEFT;
			sublevel[2] = VECTOR2_LEFT;
			sublevel[3] = VECTOR2_DOWN;
		}
		if (direction == VECTOR2_RIGHT)
		{
			connect[0] = VECTOR2_LEFT;
			connect[1] = VECTOR2_UP;
			connect[2] = VECTOR2_RIGHT;
			sublevel[0] = VECTOR2_DOWN;
			sublevel[1] = VECTOR2_RIGHT;
			sublevel[2] = VECTOR2_RIGHT;
			sublevel[3] = VECTOR2_UP;
		}
		if (direction == VECTOR2_UP)
		{
			connect[0] = VECTOR2_DOWN;
			connect[1] = VECTOR2_RIGHT;
			connect[2] = VECTOR2_UP;
			sublevel[0] = VECTOR2_LEFT;
			sublevel[1] = VECTOR2_UP;
			sublevel[2] = VECTOR2_UP;
			sublevel[3] = VECTOR2_RIGHT;
		}
		if (direction == VECTOR2_DOWN)
		{
			connect[0] = VECTOR2_UP;
			connect[1] = VECTOR2_LEFT;
			connect[2] = VECTOR2_DOWN;
			sublevel[0] = VECTOR2_RIGHT;
			sublevel[1] = VECTOR2_DOWN;
			sublevel[2] = VECTOR2_DOWN;
			sublevel[3] = VECTOR2_LEFT;
		}

		p = Hilbert(level - 1, tileSize, sublevel[0], points, p);
		points[p] = points[p - 1] + tileSize * connect[0];
		p++;
		p = Hilbert(level - 1, tileSize, sublevel[1], points, p);
		points[p] = points[p - 1] + tileSize * connect[1];
		p++;
		p = Hilbert(level - 1, tileSize, sublevel[2], points, p);
		points[p] = points[p - 1] + tileSize * connect[2];
		p++;
		p = Hilbert(level - 1, tileSize, sublevel[3], points, p);
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