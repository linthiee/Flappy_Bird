#include "player.h"

#include "raymath.h"

#include "game/game_constants.h"

using namespace Game;

namespace Player
{
	static Texture playerDownTexture;
	static Texture playerUpTexture;

	static const float DEFAULT_WIDTH = 75.0f;
	static const float DEFAULT_HEIGHT = 37.5f;

	static const float MAX_ROTATION_ANGLE = 180.0f;

	static float MIN_ROTATION_SPEED = -50;
	static float MAX_ROTATION_SPEED = -MIN_ROTATION_SPEED;

	static const float GRAVITY = 1800.0f;
	const float JUMP_STRENGTH = -650.0f;

	static void InitTexture();

	static void UpdateGravity(Player& player, float deltaTime);
	static void LimitWithBorders(Player& player);

	void Update(Player& player, float deltaTime)
	{
		UpdateGravity(player, deltaTime);

		if (player.speedY < 0.0f)
		{
			player.texture = playerDownTexture;
		}
		else
		{
			player.texture = playerUpTexture;
		}

		player.rotation = Lerp(0, MAX_ROTATION_ANGLE * DEG2RAD, Normalize(player.speedY, MIN_ROTATION_SPEED, MAX_ROTATION_SPEED));

		player.rectangle.y += player.speedY * deltaTime;

		LimitWithBorders(player);
	}

	void Draw(Player player, Color color)
	{
		float x = player.rectangle.x;
		float y = player.rectangle.y - (player.rectangle.height);

		DrawTextureEx(player.texture, { x, y }, player.rotation, 0.05f, color);
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
		newPlayer.rotation = 0.0f;
		newPlayer.score = 0;

		InitTexture();

		return newPlayer;
	}

	void Reset(Player& player, float defaultPosX, float defaultPosY)
	{
		player.rectangle.x = defaultPosX;
		player.rectangle.y = defaultPosY;
		player.speedY = 0.0f;
		player.score = 0;
	}

	void Jump(Player& player)
	{
		player.speedY = JUMP_STRENGTH;
	}

	void InitTexture()
	{
		playerUpTexture = LoadTexture("res/textures/player/raven_up.png");
		playerDownTexture = LoadTexture("res/textures/player/raven_down.png");
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

	void Close()
	{
		UnloadTexture(playerUpTexture);
		UnloadTexture(playerDownTexture);
	}
}