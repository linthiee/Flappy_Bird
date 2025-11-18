#include "obstacle.h"

#include "raylib.h"

#include "game/game_constants.h"
#include "utilities/math_utils.h"

using namespace Game;
using namespace MathUtils;

namespace Obstacle
{
	static Texture obstacleTexture1;
	static Texture obstacleTexture2;
	static Texture obstacleTexture3;

	static const float WIDTH = 150.0f;
	static const float GAP_MIN = 250.0f;
	static const float GAP_MAX = 350.0f;
	static const float SPEED_X = 500.0f;
	static const float SPAWN_X_OFFSET = 50.0f;
	static const float MARGIN_TOP = 50.0f;
	static const float MARGIN_BOTTOM = 50.0f;

	static void InitTextures();

	static void Move(Obstacle& obstacle, float deltaTime);
	static void Recycle(Obstacle& obstacle);
	static void RandomizeVerticalPosition(Obstacle& obstacle);

	void Update(Obstacle& obstacle, float deltaTime)
	{
		Move(obstacle, deltaTime);
		Recycle(obstacle);
	}

	void Draw(Obstacle obstacle)
	{
		Texture2D currentTexture;
		switch (obstacle.variant)
		{
		case 0:
		
			currentTexture = obstacleTexture1;
			break;
		
		case 1:
		
			currentTexture = obstacleTexture2;
			break;
		
		case 2:
		
			currentTexture = obstacleTexture3;
			break;
		
		default:
		
			currentTexture = obstacleTexture1;
			break;
		}

		if (currentTexture.id <= 0)
		{
			return;
		}

		float texW = static_cast<float>(currentTexture.width);
		float texH = static_cast<float>(currentTexture.height);

		Rectangle sourceTop = { 0.0f, 0.0f, texW, -texH };
		Rectangle sourceBottom = { 0.0f, 0.0f, texW, texH };

		float visualPadding = 20.0f;

		Rectangle destTop =
		{
			obstacle.rectangleTop.x - (visualPadding / 2),
			obstacle.rectangleTop.y,
			obstacle.rectangleTop.width + visualPadding,
			obstacle.rectangleTop.height
		};

		Rectangle destBottom =
		{
			obstacle.rectangleBottom.x - (visualPadding / 2),
			obstacle.rectangleBottom.y,
			obstacle.rectangleBottom.width + visualPadding,
			obstacle.rectangleBottom.height
		};

		DrawTexturePro(currentTexture, sourceTop, destTop, { 0.0f, 0.0f }, 0.0f, WHITE);
		DrawTexturePro(currentTexture, sourceBottom, destBottom, { 0.0f, 0.0f }, 0.0f, WHITE);	
	}

	Obstacle Create()
	{
		InitTextures();

		Obstacle newObstacle = {};

		RandomizeVerticalPosition(newObstacle);
		newObstacle.speedX = SPEED_X;
		newObstacle.isActive = true;

		newObstacle.variant = GetRandomValue(0, 2);

		return newObstacle;
	}

	void Reset(Obstacle& obstacle)
	{
		RandomizeVerticalPosition(obstacle);
	}

	void InitTextures()
	{
		obstacleTexture1 = LoadTexture("res/textures/obstacles/obstacle1.png");
		obstacleTexture2 = LoadTexture("res/textures/obstacles/obstacle2.png");
		obstacleTexture3 = LoadTexture("res/textures/obstacles/obstacle3.png");
	}

	static void Move(Obstacle& obstacle, float deltaTime)
	{
		obstacle.rectangleTop.x -= obstacle.speedX * deltaTime;
		obstacle.rectangleBottom.x -= obstacle.speedX * deltaTime;
	}

	static void Recycle(Obstacle& obstacle)
	{
		if (obstacle.rectangleTop.x + obstacle.rectangleTop.width < 0)
		{
			RandomizeVerticalPosition(obstacle);
			obstacle.variant = GetRandomValue(0, 2);
		}
	}

	static void RandomizeVerticalPosition(Obstacle& obstacle)
	{
		float gap = GetFloatRandomBetween(GAP_MIN, GAP_MAX);
		float maxY = static_cast<float>(SCREEN_HEIGHT) - MARGIN_BOTTOM - gap;
		float gapY = GetFloatRandomBetween(MARGIN_TOP, maxY);

		float x = static_cast<float>(SCREEN_WIDTH) + SPAWN_X_OFFSET;

		float topHeight = gapY;
		float bottomY = gapY + gap;
		float bottomHeight = static_cast<float>(SCREEN_HEIGHT) - bottomY;

		obstacle.rectangleTop.x = x;
		obstacle.rectangleTop.y = 0.0f;
		obstacle.rectangleTop.width = WIDTH;
		obstacle.rectangleTop.height = topHeight;

		obstacle.rectangleBottom.x = x;
		obstacle.rectangleBottom.y = bottomY;
		obstacle.rectangleBottom.width = WIDTH;
		obstacle.rectangleBottom.height = bottomHeight;
	}

	void Close()
	{
		UnloadTexture(obstacleTexture1);
		UnloadTexture(obstacleTexture2);
		UnloadTexture(obstacleTexture3);
	}
}