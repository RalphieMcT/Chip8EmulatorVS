// Chip8.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <vector>
#include "Display.h"
#include "Input.h"

class Chip8 {
public:
	Chip8(Display* display, Input* input);
	void load(const char* rom);
	void run();
	void reset();
	bool drawFlag = false; //true if draw to screen on this program cycle
	unsigned short opcode = 0;

	unsigned short I = 0; //register pointer
	unsigned short pc = 0x200; //program counter

	unsigned char delay_timer = 0;
	unsigned char sound_timer = 0;

	unsigned short sp = 0; //stack pointer
	std::vector<bool> key;
	Display* _display;
	Input* _input;

private:
	void drawSprite(unsigned short Vx, unsigned short Vy, unsigned short height);
	void execute();
	void resetGfx();
	void emulateCycle();
	static const uint16_t GFX_WIDTH = 64;
	static const uint16_t GFX_HEIGHT = 32;
	static const uint16_t PIXEL_SIZE = 8;
	static const uint16_t MEMORY_SIZE = 4096;
	static const uint16_t REGISTER_COUNT = 16;
	static const uint16_t STACK_SIZE = 16;
	std::vector<uint16_t> stack;
	std::vector<std::vector<uint8_t>> gfx; //screen
	std::vector<uint8_t> V; //register
	/*
	0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
	0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
	0x200-0xFFF - Program ROM and work RAM
	*/
	std::vector<uint8_t> memory;
	std::string prevRom;
};
