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
	static Color SECOND_PLAYER_COLOR = { 230, 101, 115, 255 };

	static Player::Player player;
	static Player::Player player2;

	static Obstacle::Obstacle obstacle;

	static Button::Button buttonPause;
	static Button::Button buttonResume;
	static Button::Button buttonMenu;
	static Button::Button buttonExit;
	static Button::Button buttonMute;

	static const std::string buttonPauseName = " | | ";
	static const std::string buttonResumeName = " Resume ";
	static const std::string buttonMenuName = " Menu ";
	static const std::string buttonExitName = " Exit ";
	static const std::string buttonMuteName = "Mute";

	static const float BUTTON_WIDTH = 60.0f;
	static const float BUTTON_HEIGHT = 60.0f;
	static const float BUTTON_MARGIN = 10.0f;

	static const std::string TEXT_CONTROLS = "Controls: SPACE to jump";
	static const std::string TEXT_START_GAME = "Press SPACE to jump and start the game";
	static const std::string TEXT_JUMP = "Second player is the RED raven. press UP ARROW KEY to jump";
	static const std::string TEXT_PAUSE = "Game Paused";

	static const int TUTORIAL_FONT_SIZE = 36;
	static const int TUTORIAL_TEXT_SPACING = 90;

	static const Color TUTORIAL_BACKGROUND = { 0, 0, 0, 150 };

	static Sound scoreSound;
	static Sound click;
	static Sound collision;
	static Sound outOfBonds;

	static float deltaTime;
	static bool isGameStarted;
	static bool isGamePaused;

	static void InitButton();
	static void InitSound();
	static void UpdateButton(bool& isPaused);
	static void DrawButton(bool isPaused);
	static void DrawTutorial();
	static void HandleCollisionBetweenPlayerAndObstacle(Player::Player player);
	static void HandlePlayerFloorCollision(Player::Player player);
	static void DrawScore(int score);
	static void Reset();

	void Init()
	{
		player = Player::Create(Game::DEFAULT_X_PLAYER_1, Game::DEFAULT_Y_PLAYER_1);
		player2 = Player::Create(Game::DEFAULT_X_PLAYER_2, Game::DEFAULT_Y_PLAYER_2);
		secondPlayer::Disable();

		obstacle = Obstacle::Create();

		InitButton();
		InitSound();

		deltaTime = GetFrameTime();
		isGameStarted = false;
		isGamePaused = false;
	}

	void Input()
	{
		if (IsKeyPressed(KEY_ESCAPE))
		{
			isGamePaused = !isGamePaused;
		}

		if (isGamePaused)
		{
			return;
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

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			PlaySound(click);
		}

		UpdateButton(isGamePaused);

		if (isGameStarted && !isGamePaused)
		{
			Background::Update(deltaTime);

			Player::Update(player, deltaTime);

			if (player2.isActive)
			{
				Player::Update(player2, deltaTime);
			}

			Obstacle::Update(obstacle, deltaTime);

			if (Obstacle::CheckForScore(obstacle, player.rectangle.x))
			{
				player.score++;
				PlaySound(scoreSound);
			}

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

			if (COLLISION)
			{
				PlaySound(collision);
				COLLISION = false;
			}

			if (OUT_OF_BONDS)
			{
				PlaySound(outOfBonds);
				OUT_OF_BONDS = false;
			}
		}
	}

	void Draw()
	{
		BeginDrawing();
		ClearBackground(BLACK);

		Vector2 playerCenter = { player.rectangle.x + player.rectangle.width / 2, player.rectangle.y + player.rectangle.height / 2 };

		Background::Draw();
		Background::KrakenEye::Draw(playerCenter, deltaTime);

		Player::Draw(player, WHITE);

		if (player2.isActive)
		{
			Player::Draw(player2, SECOND_PLAYER_COLOR);
		}

		Obstacle::Draw(obstacle);
		DrawScore(player.score);

		if (!isGameStarted)
		{
			DrawTutorial();
		}

		if (isGamePaused)
		{
			DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 128,128,128,128 });

			int textWidth = MeasureText(TEXT_PAUSE.c_str(), TUTORIAL_FONT_SIZE);
			int textX = (SCREEN_WIDTH - textWidth) / 2;
			int textY = (SCREEN_HEIGHT - TUTORIAL_FONT_SIZE) / 2;

			DrawText(TEXT_PAUSE.c_str(), textX, textY, TUTORIAL_FONT_SIZE, WHITE);
		}

		DrawButton(isGamePaused);

		EndDrawing();
	}

	void Close()
	{
		Obstacle::Close();
		Player::Close();
	}

	static void InitButton()
	{
		float x = static_cast<float>(SCREEN_WIDTH) - BUTTON_WIDTH - BUTTON_MARGIN;
		float y = BUTTON_MARGIN;

		buttonPause = Button::Create(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, buttonPauseName);
		buttonResume = Button::Create((SCREEN_WIDTH / 2) - 60, (SCREEN_HEIGHT / 2) + 30, BUTTON_WIDTH + 60, BUTTON_HEIGHT, buttonResumeName);
		buttonMenu = Button::Create((SCREEN_WIDTH / 2) - 60, (SCREEN_HEIGHT / 2) + 100, BUTTON_WIDTH + 60, BUTTON_HEIGHT, buttonMenuName);
		buttonExit = Button::Create((SCREEN_WIDTH / 2) - 60, (SCREEN_HEIGHT / 2) + 170, BUTTON_WIDTH + 60, BUTTON_HEIGHT, buttonExitName);
		buttonMute = Button::Create(x - 100, y, BUTTON_WIDTH + 30, BUTTON_HEIGHT, buttonMuteName);
	}

	void InitSound()
	{
		scoreSound = LoadSound("res/sound/gameplay/scored.wav");
		click = LoadSound("res/sound/menu/click.wav");
		collision = LoadSound("res/sound/gameplay/death.wav");
		outOfBonds = LoadSound("res/sound/gameplay/outofbonds.wav");
	}

	static void UpdateButton(bool& isPaused)
	{
		Button::Update(buttonPause);
		Button::Update(buttonMute);

		if (isPaused)
		{
			Button::Update(buttonResume);
			Button::Update(buttonMenu);
			Button::Update(buttonExit);

			if (buttonResume.clicked)
			{
				isPaused = false;
			}
			if (buttonMenu.clicked)
			{
				CosmicJump::currentScene = CosmicJump::Scenes::MainMenu;
			}
			if (buttonExit.clicked)
			{
				CosmicJump::currentScene = CosmicJump::Scenes::Exit;
			}
		}

		if (buttonPause.clicked)
		{
			isPaused = !isPaused;
		}

		if (buttonMute.clicked)
		{
			// mute
		}
	}

	static void DrawButton(bool isPaused)
	{
		Button::Draw(buttonPause);
		Button::Draw(buttonMute);

		if (isPaused)
		{
			Button::Draw(buttonResume);
			Button::Draw(buttonMenu);
			Button::Draw(buttonExit);
		}
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
			COLLISION = true;
		}
	}

	static void HandlePlayerFloorCollision(Player::Player currentPlayer)
	{
		if (currentPlayer.rectangle.y + currentPlayer.rectangle.height >= SCREEN_HEIGHT)
		{
			Reset();
			isGameStarted = false;
			OUT_OF_BONDS = true;
		}
	}

	void DrawScore(int score)
	{
		const char* scoreText = TextFormat("%i", score);

		int fontSize = 50;

		int textWidth = MeasureText(scoreText, fontSize);

		int x =	SCREEN_WIDTH / 2 - textWidth / 2;
		int y = 50;

		DrawText(scoreText, x + 3, y + 3, fontSize, BLACK);

		DrawText(scoreText, x, y, fontSize, WHITE);
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