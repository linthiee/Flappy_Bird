#pragma once

#include <string>

namespace Game
{
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;
	const std::string PROGRAM_NAME = "Cosmic Jump";
	const std::string PROGRAM_VERSION = "0.4";
	const int VERSION_TEXT_SIZE = 36;

	static const float DEFAULT_WIDTH = 75.0f;
	static const float DEFAULT_HEIGHT = 75.0f;

	const float DEFAULT_X_PLAYER_1 = 200.0f;
	const float DEFAULT_X_PLAYER_2 = 400.0f;

	const float DEFAULT_Y_PLAYER_1 = static_cast<float>(SCREEN_HEIGHT) / 2.0f - DEFAULT_WIDTH / 2.0f;
	const float DEFAULT_Y_PLAYER_2 = static_cast<float>(SCREEN_HEIGHT) / 4.0f - DEFAULT_WIDTH / 2.0f;
}