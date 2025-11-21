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
		bool passed;

		int variant;
	};

	void Update(Obstacle& obstacle, float deltaTime);
	void Draw(Obstacle obstacle);

	Obstacle Create();
	bool CheckForScore(Obstacle& obstacle, float birdXPosition);
	void Reset(Obstacle& obstacle);

	void Close();
}