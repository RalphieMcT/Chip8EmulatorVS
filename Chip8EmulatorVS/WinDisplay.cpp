#include "WinDisplay.h"

WinDisplay::WinDisplay(uint8_t _width, uint8_t _height) {
	width = _width;
	height = _height;
	window = SDL_CreateWindow("chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, PIXEL_SIZE * width, PIXEL_SIZE * height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	screen_rect.x = 0;
	screen_rect.y = 0;
	screen_rect.w = PIXEL_SIZE * _width;
	screen_rect.h = PIXEL_SIZE * _height;
}

WinDisplay::~WinDisplay()
{
	SDL_DestroyRenderer(renderer);
	//SDL_VideoQuit();
	SDL_DestroyWindow(window);
}

void WinDisplay::drawGraphics(std::vector<std::vector<uint8_t>> gfx)
{
	for (uint8_t y = 0; y < gfx.size(); y++)
	{
		for (uint8_t x = 0; x < gfx.at(0).size(); x++)
		{
			SDL_Rect rect = {
				x * PIXEL_SIZE,
				y * PIXEL_SIZE,
				PIXEL_SIZE,
				PIXEL_SIZE
			};

			if (gfx.at(y).at(x) > 0)
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
			}

			SDL_RenderFillRect(renderer, &rect);
		}
	}
	SDL_RenderPresent(renderer);
}
