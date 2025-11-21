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
	static const float OBSTACLE_HEIGHT = 800.0f;

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

		const float ENLARGE_FACTOR = 1.8f;

		float baseScale = obstacle.rectangleTop.width / texW;
		float scale = baseScale * ENLARGE_FACTOR;

		float visualWidth = texW * scale;
		float visualHeight = texH * scale;

		float visualPaddingX = (obstacle.rectangleTop.width - visualWidth) / 2.0f;

		{
			float gapEdgeY = obstacle.rectangleTop.y + obstacle.rectangleTop.height;

			Rectangle dest =
			{
				obstacle.rectangleTop.x + visualPaddingX,
				gapEdgeY - visualHeight,
				visualWidth,
				visualHeight
			};

			Rectangle source = { 0.0f, 0.0f, texW, -texH };

			DrawTexturePro(currentTexture, source, dest, { 0.0f, 0.0f }, 0.0f, WHITE);
		}

		{
			Rectangle dest =
			{
				obstacle.rectangleBottom.x + visualPaddingX,
				obstacle.rectangleBottom.y,
				visualWidth,
				visualHeight
			};

			Rectangle source = { 0.0f, 0.0f, texW, texH };

			DrawTexturePro(currentTexture, source, dest, { 0.0f, 0.0f }, 0.0f, WHITE);
		}
	}

	Obstacle Create()
	{
		Obstacle newObstacle = {};

		RandomizeVerticalPosition(newObstacle);
		newObstacle.speedX = SPEED_X;
		newObstacle.isActive = true;
		newObstacle.passed = false; 

		newObstacle.variant = GetRandomValue(0, 2);

		InitTextures();

		return newObstacle;
	}

	bool CheckForScore(Obstacle& obstacle, float birdXPosition)
	{
		if (obstacle.isActive && !obstacle.passed && (obstacle.rectangleTop.x + obstacle.rectangleTop.width < birdXPosition))
		{
			obstacle.passed = true;
			return true;
		}
		return false;
	}

	void Reset(Obstacle& obstacle)
	{
		RandomizeVerticalPosition(obstacle);
		obstacle.passed = false; 
	}

	void Close()
	{
		UnloadTexture(obstacleTexture1);
		UnloadTexture(obstacleTexture2);
		UnloadTexture(obstacleTexture3);
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
			obstacle.passed = false;
		}
	}

	static void RandomizeVerticalPosition(Obstacle& obstacle)
	{
		float gap = GetFloatRandomBetween(GAP_MIN, GAP_MAX);

		float maxY = static_cast<float>(SCREEN_HEIGHT) - MARGIN_BOTTOM - gap;
		float gapY = GetFloatRandomBetween(MARGIN_TOP, maxY);

		float x = static_cast<float>(SCREEN_WIDTH) + SPAWN_X_OFFSET;

		obstacle.rectangleTop.x = x;

		obstacle.rectangleTop.y = gapY - OBSTACLE_HEIGHT;
		obstacle.rectangleTop.width = WIDTH;
		obstacle.rectangleTop.height = OBSTACLE_HEIGHT;

		obstacle.rectangleBottom.x = x;

		obstacle.rectangleBottom.y = gapY + gap;
		obstacle.rectangleBottom.width = WIDTH;
		obstacle.rectangleBottom.height = OBSTACLE_HEIGHT;
	}
}