#include "Level.h"
#include <SDL_image.h>

Level::Level(int width, int height) :
	m_width(width),
	m_height(height),
	m_blocks(std::make_unique<BLOCK[]>(width*height)),
	m_player(std::make_unique<Entity>())
{
	m_player->spriteFile = "assets/images/player.png";
	m_entities.push_back(m_player.get());
}

void Level::Init()
{
	m_images["assets/images/player.png"] = IMG_Load("assets/images/player.png");
	m_images["assets/images/box.png"] = IMG_Load("assets/images/box.png");

	m_images["assets/images/floor.png"] = IMG_Load("assets/images/floor.png");
	m_images["assets/images/switch.png"] = IMG_Load("assets/images/switch.png");
	m_images["assets/images/wall.png"] = IMG_Load("assets/images/wall.png");

	m_images["assets/images/win.png"] = IMG_Load("assets/images/win.png");
}

void Level::Shutdown()
{
	for (auto it : m_images)
	{
		SDL_FreeSurface(it.second);
	}
	m_images.clear();
}

void Level::Render(SDL_Surface* buffer)
{
	int tileSize = 64;
	SDL_FillRect(buffer, NULL, SDL_MapRGB(buffer->format, 0, 0, 0));

	for (int iY = 0; iY < m_height; iY++)
	{
		for (int iX = 0; iX < m_width; iX++)
		{
			SDL_Rect rect = {iX * tileSize, iY * tileSize, tileSize, tileSize};
			SDL_Surface* src = NULL;
			switch (this->GetBlockAt(iX, iY)) {
			case BLOCK::BL_FLOOR:
				src = m_images["assets/images/floor.png"];
				break;
			case BLOCK::BL_SWITCH:
				src = m_images["assets/images/switch.png"];
				break;
			case BLOCK::BL_WALL:
				src = m_images["assets/images/wall.png"];
				break;
			}
			
			if (src != NULL) {
				SDL_BlitSurface(src, NULL, buffer, &rect);
			}
		}
	}
	for (int i = 0; i < m_entities.size(); i++)
	{
		auto e = m_entities[i];
		SDL_Rect rect = { e->x * tileSize, e->y * tileSize, tileSize, tileSize };
		SDL_Surface* src = m_images[e->spriteFile];
		if (src == NULL) {
			std::string error = "image missing: " + e->spriteFile;
			throw std::exception(error.c_str());
		}
		SDL_BlitSurface(src, NULL, buffer, &rect);
		//SDL_FillRect(buffer, &rect, SDL_MapRGB(buffer->format, 0xCC, 0xCC, 0xCC));
	}
}

void Level::Move(int xDir, int yDir)
{
	if (xDir > 1 || xDir < -1)
		throw std::exception("x can only be 1 or -1");
	if (yDir > 1 || yDir < -1)
		throw std::exception("y can only be 1 or -1");
	if (xDir != 0 && yDir != 0)
		throw std::exception("can only move on one axis at a time");

	// x == -1: left
	// x == 1: right
	// y == -1: up
	// y == 1: down

	int newX = m_player->x + xDir;
	int newY = m_player->y + yDir;
	// if player is out of bounds, not a valid move
	if (newX < 0 || newX >= m_width)
		return;
	if (newY < 0 || newY >= m_height)
		return;
	
	// else if player is in wall, not a valid move
	if (GetBlockAt(newX, newY) == BLOCK::BL_WALL)
		return;
	// else if player is colliding with box, try to move box in same direction
	Entity* box;
	if ((box = GetBoxAt(newX, newY)) != nullptr) {
		int newBoxX = box->x + xDir;
		int newBoxY = box->y + yDir;
		// if box is in wall or colliding with another box, not a valid move
		if (GetBlockAt(newBoxX, newBoxY) == BLOCK::BL_WALL || GetBoxAt(newBoxX, newBoxY) != nullptr)
			return;
		// all good, move the box and the player
		box->x = newBoxX;
		box->y = newBoxY;
		m_player->x = newX;
		m_player->y = newY;
		return;
	}
	// else if player is on a ground block, valid move
	m_player->x = newX;
	m_player->y = newY;
}

BLOCK Level::GetBlockAt(int x, int y)
{
	int offset = y * m_width + x;
	return m_blocks.get()[offset];
}

void Level::SetBlockAt(int x, int y, BLOCK blockType)
{
	int offset = y * m_width + x;
	m_blocks[offset] = blockType;
}

Entity* Level::GetBoxAt(int x, int y)
{
	// linear search
	for (auto& it : m_boxes) {
		if (it->x == x && it->y == y)
			return it.get();
	}
	return nullptr;
}

void Level::SetPlayerSpawn(int x, int y)
{
	m_player->x = x;
	m_player->y = y;
}

void Level::AddBoxAt(int x, int y)
{
	auto box = std::make_unique<Entity>();
	box->x = x;
	box->y = y;
	box->spriteFile = "assets/images/box.png";
	m_entities.push_back(box.get());
	m_boxes.push_back(std::move(box));
}

bool Level::Completed()
{
	for (auto& box : m_boxes) {
		if (GetBlockAt(box->x, box->y) != BLOCK::BL_SWITCH)
			return false;
	}
	return true;
}
