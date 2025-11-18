#pragma once

#include "geometry/rectangle.h"

namespace Obstacle
{
	struct Obstacle
	{
		Geometry::Rectangle rectangleTop;
		Geometry::Rectangle rectangleBottom;
		float speedX;
		bool isActive;

		int variant;
	};

	void Update(Obstacle& obstacle, float deltaTime);
	void Draw(Obstacle obstacle);

	Obstacle Create();
	void Reset(Obstacle& obstacle);

	void Close();
}