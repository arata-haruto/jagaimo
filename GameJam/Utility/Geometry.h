#pragma once

struct Vector2D
{
	float x;
	float y;
};

typedef Vector2D Position2D;
typedef Vector2D Size2D;

struct CapsuleCollision
{
	Position2D point[2];
	float radius;

};
