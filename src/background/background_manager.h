#pragma once

#include "raylib.h"

namespace Background
{
	namespace KrakenEye
	{
		void Draw(Vector2 playerCenter, float deltaTime);
	}
	void Init();
	void Update(float deltaTime);
	void Draw();
	void Close();
}