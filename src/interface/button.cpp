#include "button.h"

#include "raylib.h"

namespace Button
{
    static Color normalColor = DARKBROWN;
    static Color hoverColor = { 70, 57, 41, 255 };
    static Color pressedColor = { 90, 77, 61, 255 };

    static int BUTTON_TEXT_SIZE = 36;

    static bool IsMouseOverButton(Button button);

    void Update(Button& button)
    {
        button.clicked = false;

        if (IsMouseOverButton(button))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                button.state = ButtonState::Pressed;
            }
            else
            {
                if (button.state == ButtonState::Pressed)
                {
                    button.clicked = true;
                }

                button.state = ButtonState::Hover;
            }
        }
        else
        {
            button.state = ButtonState::Normal;
        }
    }

    void Draw(Button button)
    {
        Color buttonColor = normalColor;

        switch (button.state)
        {
        case ButtonState::Normal:

            buttonColor = normalColor;

            break;
        case ButtonState::Hover:

            buttonColor = hoverColor;

            break;
        case ButtonState::Pressed:

            buttonColor = pressedColor;

            break;
        default:

            // NO MORE BUTTON STATES

            break;
        }

        int buttonX = static_cast<int>(button.rectangle.x);
        int buttonY = static_cast<int>(button.rectangle.y);
        int buttonWidth = static_cast<int>(button.rectangle.width);
        int buttonHeight = static_cast<int>(button.rectangle.height);

        DrawRectangle(buttonX, buttonY, buttonWidth, buttonHeight, buttonColor);

        int textWidth = MeasureText(button.text.c_str(), BUTTON_TEXT_SIZE);
        int textX = static_cast<int>(button.rectangle.x + (button.rectangle.width - textWidth) / 2.0f);
        int textY = static_cast<int>(button.rectangle.y + (button.rectangle.height - BUTTON_TEXT_SIZE) / 2.0f);

        DrawText(button.text.c_str(), textX, textY, BUTTON_TEXT_SIZE, WHITE);
    }

    Button Create(float x, float y, float width, float height, const std::string& text)
    {
        Button button;

        button.rectangle.x = x;
        button.rectangle.y = y;
        button.rectangle.width = width;
        button.rectangle.height = height;
        button.text = text;
        button.state = ButtonState::Normal;
        button.clicked = false;

        return button;
    }

    static bool IsMouseOverButton(Button button)
    {
        Vector2 mouse = GetMousePosition();

        if (mouse.x >= button.rectangle.x &&
            mouse.x <= button.rectangle.x + button.rectangle.width &&
            mouse.y >= button.rectangle.y &&
            mouse.y <= button.rectangle.y + button.rectangle.height)
        {
            return true;
        }

        return false;
    }

}