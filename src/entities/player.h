#pragma once

#include "geometry/rectangle.h"

namespace Player
{
	struct Player
	{
		Geometry::Rectangle rectangle;
		float speedX;
		float speedY;
		bool isActive;
	};

	void Update(Player& player, float deltaTime);
	void Draw(Player player);

	Player Create(float posX, float posY);
	void Reset(Player& player, float posX, float posY);
	void Jump(Player& player);
}