#pragma once
#include "raylib.h"

#include "geometry/rectangle.h"

namespace Player
{
	struct Player
	{
		Geometry::Rectangle rectangle;
		float speedX;
		float speedY;
		bool isActive;
		int score;
		bool isColliding;
		bool wasColliding;
	};

	void Update(Player& player, float deltaTime);
	void Draw(Player player, Color color);

	Player Create(float posX, float posY);
	void Reset(Player& player, float posX, float posY);
	void Jump(Player& player);
}