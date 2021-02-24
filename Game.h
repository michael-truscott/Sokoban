#pragma once

#include <memory>
#include <SDL.h>
#include "Level.h"

class Game
{
public:
	Game();

	void Init();
	void Update(float delta);
	void Render(SDL_Surface* buffer);
	void Shutdown();

	bool Finished() { return m_finished; }

private:
	bool m_finished;
	std::unique_ptr<Level> m_level;
	SDL_Surface* m_winScreen;
};
