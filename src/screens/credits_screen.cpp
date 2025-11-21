#include "credits_screen.h"

#include "raylib.h"

#include "game/game.h"
#include "interface/button.h"
#include "game/game_constants.h"
#include "interface/ui_constants.h"

using namespace Game;
using namespace UIConstants;

namespace Credits
{
	struct Background
	{
		float x;
		float y;
		float speed;
		Texture texture;
	};

	static Background background;

	static const std::string TITLE = "Credits";

	static const std::string LABEL_DEV = "Developed by:";

	static const std::string AUTHOR_DEV_CLICK = "Jonas Canaza";
	static const std::string AUTHOR_DEV_REST = " & Eluney Mousseigne";

	static const std::string LABEL_ART = "Art by:";

	static const std::string AUTHOR_ART_CLICK = "Laura Srur";

	static const std::string LABEL_MUSIC = "Menu & Gameplay music by:";
	static const std::string AUTHOR_MUSIC = "May Genko (retro 8-bit rpg music)";

	static const std::string LABEL_SFX = "Sound effects by:";
	static const std::string AUTHOR_SFX = "Helton Yan (pixel combat)";

	static const char* URL_DEV = "https://canaza.itch.io/";
	static const char* URL_ART = "https://bananadolca55.itch.io/";
	static const char* URL_MUSIC = "https://maygenko.itch.io/retro-8-bit-rpg-music-pack-by-may-genko";
	static const char* URL_SFX = "https://heltonyan.itch.io/pixelcombat";

	static const int FONT_SIZE_TITLE = 64;
	static const int FONT_SIZE_LABEL = 24;
	static const int FONT_SIZE_AUTHOR = 32; 

	static Button::Button buttonBack;
	static const std::string buttonName = "Back";

	static const int BUTTON_MARGIN_BOTTOM = 20;

	static const int SECTION_SPACING = 85; 
	static const int LINE_SPACING = 35;    

	static void InitBackground();
	static void DrawBackground();
	static int GetCenteredX(const std::string& text, int fontSize);
	static void DrawInfo();
	static void InitButton();
	static void UpdateButton();
	static void DrawButton();

	static void DrawClickableText(const std::string& text, int x, int y, int fontSize, const char* url);

	void Init()
	{
		InitButton();
		InitBackground();
	}

	void Input()
	{
		if (IsKeyPressed(KEY_ESCAPE))
		{
			CosmicJump::currentScene = CosmicJump::Scenes::MainMenu;
		}
	}

	void Update()
	{
		UpdateButton();
	}

	void Draw()
	{
		ClearBackground(BLACK);
		BeginDrawing();

		DrawBackground();
		DrawInfo();
		DrawButton();

		EndDrawing();
	}

	void Close()
	{
		UnloadTexture(background.texture);
	}

	static void InitBackground()
	{
		background.x = 0.0f;
		background.y = 0.0f;
		background.texture = LoadTexture("res/textures/backgrounds/gameplay/back.png");
	}

	static void DrawBackground()
	{
		DrawTexture(background.texture, static_cast<int>(background.x), static_cast<int>(background.y), WHITE);
	}

	static int GetCenteredX(const std::string& text, int fontSize)
	{
		return (SCREEN_WIDTH - MeasureText(text.c_str(), fontSize)) / 2;
	}

	static void DrawInfo()
	{
		int titleWidth = MeasureText(TITLE.c_str(), FONT_SIZE_TITLE);
		int titleX = (SCREEN_WIDTH - titleWidth) / 2;
		int titleY = 50;
		DrawText(TITLE.c_str(), titleX, titleY, FONT_SIZE_TITLE, WHITE);

		int currentY = titleY + FONT_SIZE_TITLE + 50;
		
		DrawText(LABEL_DEV.c_str(), GetCenteredX(LABEL_DEV, FONT_SIZE_LABEL), currentY, FONT_SIZE_LABEL, LIGHTGRAY);
		currentY += LINE_SPACING;

		{
			int widthClick = MeasureText(AUTHOR_DEV_CLICK.c_str(), FONT_SIZE_AUTHOR);
			int widthRest = MeasureText(AUTHOR_DEV_REST.c_str(), FONT_SIZE_AUTHOR);
			int totalWidth = widthClick + widthRest;

			int startX = (SCREEN_WIDTH - totalWidth) / 2;

			DrawClickableText(AUTHOR_DEV_CLICK, startX, currentY, FONT_SIZE_AUTHOR, URL_DEV);
			DrawText(AUTHOR_DEV_REST.c_str(), startX + widthClick, currentY, FONT_SIZE_AUTHOR, WHITE);
		}
		currentY += SECTION_SPACING;

		DrawText(LABEL_ART.c_str(), GetCenteredX(LABEL_ART, FONT_SIZE_LABEL), currentY, FONT_SIZE_LABEL, LIGHTGRAY);
		currentY += LINE_SPACING;

		{
			int widthClick = MeasureText(AUTHOR_ART_CLICK.c_str(), FONT_SIZE_AUTHOR);
			int totalWidth = widthClick;

			int startX = (SCREEN_WIDTH - totalWidth) / 2;

			DrawClickableText(AUTHOR_ART_CLICK, startX, currentY, FONT_SIZE_AUTHOR, URL_ART);
		}
		currentY += SECTION_SPACING;

		DrawText(LABEL_MUSIC.c_str(), GetCenteredX(LABEL_MUSIC, FONT_SIZE_LABEL), currentY, FONT_SIZE_LABEL, LIGHTGRAY);
		currentY += LINE_SPACING;
		DrawClickableText(AUTHOR_MUSIC, GetCenteredX(AUTHOR_MUSIC, FONT_SIZE_AUTHOR), currentY, FONT_SIZE_AUTHOR, URL_MUSIC);
		currentY += SECTION_SPACING;

		DrawText(LABEL_SFX.c_str(), GetCenteredX(LABEL_SFX, FONT_SIZE_LABEL), currentY, FONT_SIZE_LABEL, LIGHTGRAY);
		currentY += LINE_SPACING;
		DrawClickableText(AUTHOR_SFX, GetCenteredX(AUTHOR_SFX, FONT_SIZE_AUTHOR), currentY, FONT_SIZE_AUTHOR, URL_SFX);
	}

	static void InitButton()
	{
		float x = static_cast<float>(SCREEN_WIDTH) / 2.0f - BUTTON_WIDTH / 2.0f;
		float y = static_cast<float>(SCREEN_HEIGHT) - BUTTON_HEIGHT - BUTTON_MARGIN_BOTTOM;

		buttonBack = Button::Create(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, buttonName);
	}

	static void UpdateButton()
	{
		Button::Update(buttonBack);

		if (buttonBack.clicked)
		{
			CosmicJump::currentScene = CosmicJump::Scenes::MainMenu;
		}
	}

	static void DrawButton()
	{
		Button::Draw(buttonBack);
	}

	static void DrawClickableText(const std::string& text, int x, int y, int fontSize, const char* url)
	{
		int textWidth = MeasureText(text.c_str(), fontSize);

		Rectangle textRec =
		{
			static_cast<float>(x),
			static_cast<float>(y),
			static_cast<float>(textWidth),
			static_cast<float>(fontSize)
		};

		Vector2 mousePoint = GetMousePosition();
		bool isHovering = CheckCollisionPointRec(mousePoint, textRec);

		Color textColor = WHITE;

		if (isHovering)
		{
			textColor = SKYBLUE;

			SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);

			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			{
				OpenURL(url);
			}
		}

		DrawText(text.c_str(), x, y, fontSize, textColor);

		if (isHovering)
		{
			DrawLine(x, y + fontSize, x + textWidth, y + fontSize, SKYBLUE);
		}
	}
}