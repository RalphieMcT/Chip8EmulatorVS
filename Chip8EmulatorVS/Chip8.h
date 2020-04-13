// Chip8.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <SDL.h>
#include <vector>

class Chip8 {
public:
	Chip8();
	~Chip8();
	void load();
	void run();
	void drawSprite(unsigned short Vx, unsigned short Vy, unsigned short height);
	void clearScreen();
	void execute();
	void emulateCycle();
	void drawGraphics(); //draw to screen
	void setKeys();
	bool drawFlag = false; //true if draw to screen on this program cycle
	unsigned short opcode = 0;

	unsigned short I = 0; //register pointer
	unsigned short pc = 0x200; //program counter

	unsigned char delay_timer = 0;
	unsigned char sound_timer = 0;

	unsigned short sp = 0; //stack pointer
	std::vector<bool> key;

	SDL_Rect screen_rect;
	SDL_Window* window;
	SDL_Renderer* renderer;

private:
	static const uint16_t GFX_WIDTH = 64;
	static const uint16_t GFX_HEIGHT = 32;
	static const uint16_t PIXEL_SIZE = 8;
	static const uint16_t MEMORY_SIZE = 4096;
	static const uint16_t REGISTER_COUNT = 16;
	static const uint16_t STACK_SIZE = 16;
	std::vector<uint16_t> stack;
	std::vector<uint8_t> gfx; //screen
	std::vector<uint8_t> V; //register
	std::vector<uint8_t> memory;
};
