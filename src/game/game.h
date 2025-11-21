#pragma once

namespace CosmicJump
{
	enum class Scenes
	{
		MainMenu,
		Gameplay,
		Credits,
		Exit
	};

	extern Scenes currentScene;
	extern bool isRunning;

	void Play();
}