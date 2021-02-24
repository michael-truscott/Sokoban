#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <chrono>
#include <memory>
#include "Game.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

bool Init();
void GameLoop();
void Shutdown();

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;

std::unique_ptr<Game> game;

int main(int argc, char *argv[]) {
	if (!Init())
	{
		// TODO: error message or something
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Init Failed", SDL_GetError(), NULL);
		return 1;
	}

	try
	{
		GameLoop();
	}
	catch (std::exception& e)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", e.what(), NULL);
	}
	Shutdown();
	return 0;
}

bool Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return false;
	}
	window = SDL_CreateWindow("Sokoban", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
										SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		return false;
	}
	screenSurface = SDL_GetWindowSurface(window);
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0x00, 0xFF));
	SDL_UpdateWindowSurface(window);

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		return false;
	}

	game = std::make_unique<Game>();
	game->Init();
	return true;
}

void GameLoop()
{
	auto prevTime = std::chrono::steady_clock::now();
	auto currentTime = std::chrono::steady_clock::now();

	while (!game->Finished()) {
		currentTime = std::chrono::steady_clock::now();
		int elapsedMs = (int)std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - prevTime).count();
		if (elapsedMs == 0)
			continue;
		float dt = elapsedMs / 1000.0f;
		prevTime = std::chrono::steady_clock::now();

		game->Update(dt);
		game->Render(screenSurface);
		SDL_UpdateWindowSurface(window);
	}
}

void Shutdown()
{
	game->Shutdown();

	SDL_DestroyWindow(window);
	SDL_Quit();
}
