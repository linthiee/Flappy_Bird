#include "background/background_manager.h"

#include "raylib.h"

#include "game/game_constants.h"

#include <cmath>

using namespace Game;

namespace Background
{
	static const float PARALLAX_SPEED_BACK = 45.0f;
	static const float PARALLAX_SPEED_MID = 145.0f;
	static const float PARALLAX_SPEED_FRONT = 360.0f;
	static const float PARALLAX_SPEED_GARBAGE = 400.0f;

	const Vector2 EYE_CENTER_POSITION = { 1110.0f, 270.0f };

	const float EYE_RADIUS_X = 15.0f;
	const float EYE_RADIUS_Y = 50.0f;
	const float PUPIL_SPEED = 5.0f;

	struct Background
	{
		float x;
		float y;
		float speed;
		Texture texture;
	};

	static Background gameplayBack;
	static Background gameplayMid;
	static Background gameplayFront;
	static Background gameplayFront2;

	static Background garbage;
	static Background pupil;

	static float backgroundScale;
	static float limit;

	static void InitBackground();
	static void UpdateBackground(float deltaTime);
	static void DrawBackground();

	void Init()
	{
		InitBackground();
	}

	void Update(float deltaTime)
	{
		UpdateBackground(deltaTime);
	}

	void Draw()
	{
		DrawBackground();
	}

	void Close()
	{
		UnloadTexture(gameplayBack.texture);
		UnloadTexture(gameplayMid.texture);
		UnloadTexture(gameplayFront.texture);
		UnloadTexture(gameplayFront2.texture);
		UnloadTexture(garbage.texture);
	}

	static void InitBackground()
	{
		gameplayBack.texture = LoadTexture("res/textures/backgrounds/gameplay/back.png");
		gameplayMid.texture = LoadTexture("res/textures/backgrounds/gameplay/mid.png");
		gameplayFront.texture = LoadTexture("res/textures/backgrounds/gameplay/front.png");
		gameplayFront2.texture = LoadTexture("res/textures/backgrounds/gameplay/front2.png");
		garbage.texture = LoadTexture("res/textures/backgrounds/gameplay/garbage.png");
		pupil.texture = LoadTexture("res/textures/backgrounds/gameplay/pupil.png");

		gameplayBack.x = 0.0f;
		gameplayBack.y = 0.0f;
		gameplayBack.speed = PARALLAX_SPEED_BACK;

		gameplayMid.x = 0.0f;
		gameplayMid.y = 0.0f;
		gameplayMid.speed = PARALLAX_SPEED_MID;

		gameplayFront.x = 0.0f;
		gameplayFront.y = 0.0f;
		gameplayFront.speed = PARALLAX_SPEED_FRONT;

		garbage.x = 0.0f;
		garbage.y = 400.0f;
		garbage.speed = PARALLAX_SPEED_GARBAGE;

		gameplayFront2.x = 0.0f;
		gameplayFront2.y = 0.0f;
		gameplayFront2.speed = PARALLAX_SPEED_FRONT;

		backgroundScale = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(gameplayBack.texture.width);
		limit = static_cast<float>(gameplayBack.texture.width) * backgroundScale;
	}

	static void UpdateBackground(float deltaTime)
	{
		gameplayBack.x -= gameplayBack.speed * deltaTime;
		gameplayMid.x -= gameplayMid.speed * deltaTime;
		gameplayFront.x -= gameplayFront.speed * deltaTime;
		gameplayFront2.x -= gameplayFront.speed * deltaTime;
		garbage.x -= garbage.speed * deltaTime;

		if (gameplayBack.x <= -limit)
		{
			gameplayBack.x += limit;
		}

		if (garbage.x <= -limit)
		{
			garbage.x += limit;
		}

		if (gameplayFront.x <= -limit)
		{
			gameplayFront.x += limit;
		}

		if (gameplayFront2.x <= -limit)
		{
			gameplayFront2.x += limit;
		}
	}

	static void DrawBackground()
	{
		Vector2 backPosOne = { gameplayBack.x, gameplayBack.y };
		Vector2 backPosTwo = { gameplayBack.x + limit, gameplayBack.y };

		Vector2 midPosOne = { gameplayMid.x, gameplayMid.y };
		//	Vector2 midPosTwo = { gameplayMid.x + limit, gameplayMid.y };

		Vector2 frontPosOne = { gameplayFront.x, gameplayFront.y };
		Vector2 frontPosTwo = { gameplayFront.x + limit, gameplayFront.y };

		Vector2 front2PosOne = { gameplayFront2.x, gameplayFront2.y };
		Vector2 front2PosTwo = { gameplayFront2.x + limit, gameplayFront2.y };

		Vector2 garbagePosOne = { garbage.x, garbage.y };
		Vector2 garbagePosTwo = { garbage.x + limit, garbage.y };

		DrawTextureEx(gameplayBack.texture, backPosOne, 0.0f, backgroundScale, WHITE);
		DrawTextureEx(gameplayBack.texture, backPosTwo, 0.0f, backgroundScale, WHITE);

		DrawTextureEx(gameplayMid.texture, { 50.0f, 0.0f }, 0.0f, backgroundScale, WHITE);
		//DrawTextureEx(gameplayMid.texture, midPosTwo, 0.0f, backgroundScale, WHITE);

		DrawTextureEx(garbage.texture, garbagePosOne, 0.0f, backgroundScale, WHITE);
		DrawTextureEx(garbage.texture, garbagePosTwo, 0.0f, backgroundScale, WHITE);

		DrawTextureEx(gameplayFront.texture, frontPosOne, 0.0f, backgroundScale, WHITE);
		DrawTextureEx(gameplayFront.texture, frontPosTwo, 0.0f, backgroundScale, WHITE);

		DrawTextureEx(gameplayFront2.texture, front2PosOne, 0.0f, backgroundScale, WHITE);
		DrawTextureEx(gameplayFront2.texture, front2PosTwo, 0.0f, backgroundScale, WHITE);
	}

	void KrakenEye::Draw(Vector2 playerPosition, float deltaTime)
	{
		static Vector2 currentPupilPos = EYE_CENTER_POSITION;

		float dx = playerPosition.x - EYE_CENTER_POSITION.x;
		float dy = playerPosition.y - EYE_CENTER_POSITION.y;
		float angle = atan2(dy, dx); 

		float targetX = EYE_CENTER_POSITION.x + cos(angle) * EYE_RADIUS_X;
		float targetY = EYE_CENTER_POSITION.y + sin(angle) * EYE_RADIUS_Y;

		currentPupilPos.x += (targetX - currentPupilPos.x) * PUPIL_SPEED * deltaTime;
		currentPupilPos.y += (targetY - currentPupilPos.y) * PUPIL_SPEED * deltaTime;

		Vector2 drawPos = {};
		drawPos.x = currentPupilPos.x - (pupil.texture.width / 2.0f);
		drawPos.y = currentPupilPos.y - (pupil.texture.height / 2.0f);

		DrawTextureV(pupil.texture, drawPos, WHITE);
	}
}
