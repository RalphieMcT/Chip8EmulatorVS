// Chip8.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

class Chip8 {
public:
	Chip8();
	~Chip8();
	void load();
	void run();
	void drawSprite(unsigned short Vx, unsigned short , unsigned short, unsigned short);
	void execute();
	void emulateCycle();
	void drawGraphics(); //draw to screen
	bool drawFlag = false; //true if draw to screen on this program cycle
	unsigned short opcode = 0;
	unsigned char memory[4096] = {};

	unsigned char V[16] = {}; //register
	unsigned short I = 0; //register pointer
	unsigned short pc = 0x200; //program counter
	unsigned char gfx[64 * 32] = {}; //screen

	unsigned char delay_timer = 0;
	unsigned char sound_timer = 0;

	unsigned short stack[16] = {};
	unsigned short sp = 0; //stack pointer
	unsigned char key[16] = {};
};
