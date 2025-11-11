#include "player.h"

#include "raylib.h"

#include "game/game_constants.h"

using namespace Game;

namespace Player
{
	static const float DEFAULT_WIDTH = 75.0f;
	static const float DEFAULT_HEIGHT = 75.0f;

	static const float GRAVITY = 1800.0f;
	const float JUMP_STRENGTH = -650.0f;

	static void UpdateGravity(Player& player, float deltaTime);
	static void LimitWithBorders(Player& player);

	void Update(Player& player, float deltaTime)
	{
		UpdateGravity(player, deltaTime);

		player.rectangle.y += player.speedY * deltaTime;

		LimitWithBorders(player);
	}

	void Draw(Player player)
	{
		int x = static_cast<int>(player.rectangle.x);
		int y = static_cast<int>(player.rectangle.y);
		int width = static_cast<int>(player.rectangle.width);
		int height = static_cast<int>(player.rectangle.height);

		DrawRectangle(x, y, width, height, GREEN);
	}

	Player Create(float posX, float posY)
	{
		Player newPlayer = {};

		newPlayer.rectangle.x = posX;
		newPlayer.rectangle.y = posY;
		newPlayer.rectangle.width = DEFAULT_WIDTH;
		newPlayer.rectangle.height = DEFAULT_HEIGHT;
		newPlayer.speedX = 0.0f;
		newPlayer.speedY = 0.0f;
		newPlayer.isActive = true;

		return newPlayer;
	}

	void Reset(Player& player, float defaultPosX, float defaultPosY)
	{
		player.rectangle.x = defaultPosX;
		player.rectangle.y = defaultPosY;
		player.speedY = 0.0f;
	}

	void Jump(Player& player)
	{
		player.speedY = JUMP_STRENGTH;
	}

	static void UpdateGravity(Player& player, float deltaTime)
	{
		player.speedY += GRAVITY * deltaTime;
	}

	static void LimitWithBorders(Player& player)
	{
		if (player.rectangle.y < 0.0f)
		{
			player.rectangle.y = 0.0f;
			player.speedY = 0.0f;
		}

		if (player.rectangle.y + DEFAULT_HEIGHT > SCREEN_HEIGHT)
		{
			player.rectangle.y = SCREEN_HEIGHT - DEFAULT_HEIGHT;
			player.speedY = 0.0f;
		}
	}
}