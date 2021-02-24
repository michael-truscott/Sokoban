#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <SDL.h>
#include "Entity.h"

enum class BLOCK {
	BL_EMPTY,
	BL_WALL,
	BL_FLOOR,
	BL_SWITCH,
};

class Level
{
public:
	Level(int width, int height);

	void Init();
	void Shutdown();

	void Render(SDL_Surface* buffer);
	void Move(int xDir, int yDir);
	BLOCK GetBlockAt(int x, int y);
	void SetBlockAt(int x, int y, BLOCK blockType);

	Entity* GetBoxAt(int x, int y);

	void SetPlayerSpawn(int x, int y);
	void AddBoxAt(int x, int y);

	int Width() { return m_width; }
	int Height() { return m_height; }

	bool Completed();
private:
	std::unique_ptr<BLOCK[]> m_blocks;
	std::unique_ptr<Entity> m_player;
	std::vector<std::unique_ptr<Entity>> m_boxes;

	std::vector<Entity*> m_entities; // for rendering only
	int m_width, m_height;

	std::unordered_map<std::string, SDL_Surface*> m_images;
};

