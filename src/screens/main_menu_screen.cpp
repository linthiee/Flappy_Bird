#include "main_menu_screen.h"

#include "raylib.h"

#include "game/game.h"
#include "interface/button.h"
#include "game/game_constants.h"
#include "interface/ui_constants.h"
#include "screens/gameplay_screen.h"

using namespace Game;
using namespace UIConstants;

namespace MainMenu
{
	static Texture logoTexture;

	static Geometry::Rectangle logo;

	static const int MAX_BUTTONS = 4;
	static Button::Button buttons[MAX_BUTTONS];
	static const std::string buttonNames[MAX_BUTTONS] = { "Play", "2 Players", "Credits","Exit" };

	enum ButtonID
	{
		Play,
		Multiplayer,
		Credits,
		Exit,
	};

	struct Background
	{
		float x;
		float y;
		float speed;
		Texture texture;
	};

	static Background background;

	static void InitLogo();
	static void InitButtons();
	static void InitBackground();
	static void UpdateButtons();
	static void DrawLogo();
	static void DrawButtons();
	static void DrawBackground();
	static void DrawVersion();
	static float GetTotalMenuHeight();

	void Init()
	{
		InitBackground();
		InitLogo();
		InitButtons();
	}

	void Input()
	{
	}

	void Update()
	{
		UpdateButtons();
	}

	void Draw()
	{
		ClearBackground(BLACK);
		BeginDrawing();

		DrawBackground();
		DrawLogo();
		DrawButtons();
		DrawVersion();

		EndDrawing();
	}

	void Close()
	{
		UnloadTexture(background.texture);
		UnloadTexture(logoTexture);
	}

	static void InitLogo()
	{
		float totalMenuHeight = GetTotalMenuHeight();
		float logoStartY = (SCREEN_HEIGHT - totalMenuHeight) / 2.0f;

		logo.x = static_cast<float>(SCREEN_WIDTH) / 2.0f - LOGO_WIDTH / 2.0f;
		logo.y = logoStartY;
		logo.width = LOGO_WIDTH;
		logo.height = LOGO_HEIGHT;

		logoTexture = LoadTexture("res/textures/logo/cosmic_jump.png");
	}

	static void InitButtons()
	{
		float totalMenuHeight = GetTotalMenuHeight();
		float logoStartY = (SCREEN_HEIGHT - totalMenuHeight) / 2.0f;
		float buttonsStartY = logoStartY + LOGO_HEIGHT + LOGO_TO_BUTTONS_MARGIN;

		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			float buttonCenterX = logo.x + LOGO_WIDTH / 2.0f;
			float buttonX = buttonCenterX - BUTTON_WIDTH / 2.0f;
			float buttonY = buttonsStartY + (BUTTON_HEIGHT + BUTTON_MARGIN_Y) * i;

			buttons[i] = Button::Create(buttonX, buttonY, BUTTON_WIDTH, BUTTON_HEIGHT, buttonNames[i]);
		}
	}

	static void InitBackground()
	{
		background.texture = LoadTexture("res/textures/backgrounds/gameplay/back.png");
		background.x = 0.0f;
		background.y = 0.0f;
	}

	static void DrawLogo()
	{
		int x = static_cast<int>(logo.x);
		int y = static_cast<int>(logo.y);

		DrawTextureEx(logoTexture, { static_cast<float>(x), static_cast<float>(y) }, 0.0f, 0.4f, WHITE);
	}

	static void DrawButtons()
	{
		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			Button::Draw(buttons[i]);
		}
	}

	static void DrawBackground()
	{
		DrawTexture(background.texture, static_cast<int>(background.x), static_cast<int>(background.y), WHITE);
	}

	static void UpdateButtons()
	{
		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			Button::Update(buttons[i]);
		}

		if (buttons[Play].clicked)
		{
			Gameplay::secondPlayer::Disable();
			CosmicJump::currentScene = CosmicJump::Scenes::Gameplay;
		}

		if (buttons[Credits].clicked)
		{
			CosmicJump::currentScene = CosmicJump::Scenes::Credits;
		}

		if (buttons[Multiplayer].clicked)
		{
			Gameplay::secondPlayer::Enable();
			CosmicJump::currentScene = CosmicJump::Scenes::Gameplay;	
		}

		if (buttons[Exit].clicked)
		{
			CosmicJump::isRunning = false;
		}
	}

	static void DrawVersion()
	{
		int textWidth = MeasureText(PROGRAM_VERSION.c_str(), VERSION_TEXT_SIZE);
		int margin = 10;

		int x = SCREEN_WIDTH - textWidth - margin;
		int y = SCREEN_HEIGHT - VERSION_TEXT_SIZE;

		DrawText(PROGRAM_VERSION.c_str(), x, y, VERSION_TEXT_SIZE, WHITE);
	}

	static float GetTotalMenuHeight()
	{
		return LOGO_HEIGHT + LOGO_TO_BUTTONS_MARGIN + (BUTTON_HEIGHT * MAX_BUTTONS) + (BUTTON_MARGIN_Y * (MAX_BUTTONS - 1));
	}
}
