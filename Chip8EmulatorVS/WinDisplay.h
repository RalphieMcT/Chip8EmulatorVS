#pragma once
#include <SDL.h>
#include "Chip8.h"
#include <vector>
#include "Display.h"

class WinDisplay : public Display{
public:
	WinDisplay(uint8_t width, uint8_t height);
	~WinDisplay();
	void drawGraphics(std::vector<std::vector<uint8_t>> gfx);

	SDL_Rect screen_rect;
	SDL_Window* window;
	SDL_Renderer* renderer;
	const uint8_t PIXEL_SIZE = 8;
private:
	uint8_t width;
	uint8_t height;
};

