#include "screens/gameplay_screen.h"

#include "raylib.h"

#include "game/game.h"
#include "game/game_constants.h"
#include "entities/player.h"
#include "entities/obstacle.h"
#include "collision/collision_manager.h"
#include "interface/button.h"
#include "background/background_manager.h"

using namespace Game;
using namespace Collision;

namespace Gameplay
{
	static Player::Player player;
	static Player::Player player2;

	static Obstacle::Obstacle obstacle;

	static Button::Button button;
	static const std::string buttonName = " | | ";

	static const float BUTTON_WIDTH = 60.0f;
	static const float BUTTON_HEIGHT = 60.0f;
	static const float BUTTON_MARGIN = 10.0f;

	static const std::string TEXT_CONTROLS = "Controls: SPACE to jump";
	static const std::string TEXT_START_GAME = "Press SPACE to jump and start the game";
	static const std::string TEXT_JUMP = "Second player press UP ARROW KEY to jump";

	static const int TUTORIAL_FONT_SIZE = 36;
	static const int TUTORIAL_TEXT_SPACING = 90;

	static const Color TUTORIAL_BACKGROUND = { 0, 0, 0, 150 };

	static float deltaTime;
	static bool isGameStarted;

	static void InitButton();
	static void UpdateButton();
	static void DrawButton();
	static void DrawTutorial();
	static void HandleCollisionBetweenPlayerAndObstacle(Player::Player player);
	static void HandlePlayerFloorCollision(Player::Player player);
	static void Reset();

	void Init()
	{
		player = Player::Create(Game::DEFAULT_X_PLAYER_1, Game::DEFAULT_Y_PLAYER_1);
		player2 = Player::Create(Game::DEFAULT_X_PLAYER_2, Game::DEFAULT_Y_PLAYER_2);
		secondPlayer::Disable();

		obstacle = Obstacle::Create();

		InitButton();

		deltaTime = GetFrameTime();
		isGameStarted = false;
	}

	void Input()
	{
		if (IsKeyPressed(KEY_ESCAPE))
		{
			CosmicJump::currentScene = CosmicJump::Scenes::MainMenu;
		}

		if (IsKeyPressed(KEY_SPACE) && isGameStarted)
		{
			Player::Jump(player);
		}
		else
		{
			if (IsKeyPressed(KEY_SPACE) && !isGameStarted)
			{
				isGameStarted = true;
				Player::Jump(player);
			}
		}

		if (player2.isActive && IsKeyPressed(KEY_UP) && isGameStarted)
		{
			Player::Jump(player2);
		}
	}

	void Update()
	{
		deltaTime = GetFrameTime();

		if (isGameStarted)
		{
			Background::Update(deltaTime);

			Player::Update(player, deltaTime);

			if (player2.isActive)
			{
				Player::Update(player2, deltaTime);
			}

			Obstacle::Update(obstacle, deltaTime);

			HandleCollisionBetweenPlayerAndObstacle(player);
			if (player2.isActive)
			{
				HandleCollisionBetweenPlayerAndObstacle(player2);
			}

			HandlePlayerFloorCollision(player);
			if (player2.isActive)
			{
				HandlePlayerFloorCollision(player2);
			}
		}

		UpdateButton();
	}

	void Draw()
	{
		BeginDrawing();
		ClearBackground(BLACK);

		Background::Draw();

		Player::Draw(player, GREEN);

		if (player2.isActive)
		{
			Player::Draw(player2, MAGENTA);
		}

		Obstacle::Draw(obstacle);

		if (!isGameStarted)
		{
			DrawTutorial();
		}

		DrawButton();

		EndDrawing();
	}

	void Close()
	{
		Obstacle::Close();
	}

	static void InitButton()
	{
		float x = static_cast<float>(SCREEN_WIDTH) - BUTTON_WIDTH - BUTTON_MARGIN;
		float y = BUTTON_MARGIN;

		button = Button::Create(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, buttonName);
	}

	static void UpdateButton()
	{
		Button::Update(button);

		if (button.clicked)
		{
			CosmicJump::currentScene = CosmicJump::Scenes::MainMenu;
		}
	}

	static void DrawButton()
	{
		Button::Draw(button);
	}

	static void DrawTutorial()
	{
		int textControlsWidth = MeasureText(TEXT_CONTROLS.c_str(), TUTORIAL_FONT_SIZE);
		int textJumpWidth = MeasureText(TEXT_JUMP.c_str(), TUTORIAL_FONT_SIZE);
		int textStartGameWidth = MeasureText(TEXT_START_GAME.c_str(), TUTORIAL_FONT_SIZE);

		int textControlsX = (SCREEN_WIDTH - textControlsWidth) / 2;
		int textStartGameX = (SCREEN_WIDTH - textStartGameWidth) / 2;
		int textJumpX = (SCREEN_WIDTH - textJumpWidth) / 2;

		int totalBlockHeight = TUTORIAL_FONT_SIZE + TUTORIAL_TEXT_SPACING + TUTORIAL_FONT_SIZE;

		int blockTopY = (SCREEN_HEIGHT - totalBlockHeight) / 2;

		int textControlsY = blockTopY;
		int textStartGameY = textControlsY + TUTORIAL_FONT_SIZE + TUTORIAL_TEXT_SPACING;
		int textJumpY = textControlsY + 50 + TUTORIAL_FONT_SIZE + TUTORIAL_TEXT_SPACING;

		DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, TUTORIAL_BACKGROUND);
		DrawText(TEXT_CONTROLS.c_str(), textControlsX, textControlsY, TUTORIAL_FONT_SIZE, WHITE);
		DrawText(TEXT_START_GAME.c_str(), textStartGameX, textStartGameY, TUTORIAL_FONT_SIZE, WHITE);

		if (player2.isActive)
		{
			DrawText(TEXT_JUMP.c_str(), textJumpX, textJumpY, TUTORIAL_FONT_SIZE, WHITE);
		}
	}

	static void HandleCollisionBetweenPlayerAndObstacle(Player::Player currentPlayer)
	{
		if (CheckCollisionRectangle(currentPlayer.rectangle, obstacle.rectangleTop) ||
			CheckCollisionRectangle(currentPlayer.rectangle, obstacle.rectangleBottom))
		{
			Reset();
			isGameStarted = false;
		}
	}

	static void HandlePlayerFloorCollision(Player::Player currentPlayer)
	{
		if (currentPlayer.rectangle.y + currentPlayer.rectangle.height >= SCREEN_HEIGHT)
		{
			Reset();
			isGameStarted = false;
		}
	}

	static void Reset()
	{
		Player::Reset(player, Game::DEFAULT_X_PLAYER_1, Game::DEFAULT_Y_PLAYER_1);
		Player::Reset(player2, Game::DEFAULT_X_PLAYER_2, Game::DEFAULT_Y_PLAYER_2);

		Obstacle::Reset(obstacle);
	}

	void secondPlayer::Enable()
	{
		player2.isActive = true;
	}

	void secondPlayer::Disable()
	{
		player2.isActive = false;
	}
}