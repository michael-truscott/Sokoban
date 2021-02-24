#include "Game.h"
#include <SDL_image.h>

std::unique_ptr<Level> MakeLevel() {
	// Legend:
	// (space) = empty space
	// # = wall
	// . = floor
	// o = box switch
	// b = box
	// B = box (starting on a switch)
	// P = player start
	const char* level =
		"  ##### "
		"###...# "
		"#oPb..# "
		"###.bo# "
		"#o##b.# "
		"#.#.o.##"
		"#b.Bbbo#"
		"#...o..#"
		"########";

	int width = 8, height = 9;
	auto result = std::make_unique<Level>(width, height);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			switch (level[y * width + x]) {
			case ' ':
				result->SetBlockAt(x, y, BLOCK::BL_EMPTY);
				break;
			case '#':
				result->SetBlockAt(x, y, BLOCK::BL_WALL);
				break;
			case '.':
				result->SetBlockAt(x, y, BLOCK::BL_FLOOR);
				break;
			case 'o':
				result->SetBlockAt(x, y, BLOCK::BL_SWITCH);
				break;
			case 'b':
				result->AddBoxAt(x, y);
				result->SetBlockAt(x, y, BLOCK::BL_FLOOR);
				break;
			case 'B':
				result->AddBoxAt(x, y);
				result->SetBlockAt(x, y, BLOCK::BL_SWITCH);
				break;
			case 'P':
				result->SetPlayerSpawn(x, y);
				result->SetBlockAt(x, y, BLOCK::BL_FLOOR);
				break;
			}
		}
	}
	return result;
}

Game::Game() :
	m_finished(false)
{
	m_winScreen = IMG_Load("assets/images/winner.png");
}

void Game::Init()
{
	m_level = MakeLevel();
	m_level->Init();
}

void Game::Update(float delta)
{
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				m_finished = true;
				break;
			case SDLK_UP:
				m_level->Move(0, -1);
				break;
			case SDLK_DOWN:
				m_level->Move(0, 1);
				break;
			case SDLK_LEFT:
				m_level->Move(-1, 0);
				break;
			case SDLK_RIGHT:
				m_level->Move(1, 0);
				break;
			case SDLK_r:
				Init();
				break;
			}
			break;
		case SDL_QUIT:
			m_finished = true;
			break;
		}
	}
}

void Game::Render(SDL_Surface* buffer)
{
	m_level->Render(buffer);

	if (m_level->Completed()) {
		SDL_Rect rect = { 128, 64, m_winScreen->w, m_winScreen->h };
		SDL_BlitSurface(m_winScreen, NULL, buffer, &rect);
	}
}

void Game::Shutdown()
{
	m_level->Shutdown();
	SDL_FreeSurface(m_winScreen);
}
