// Chip8.cpp : Defines the entry point for the application.
//
/*
0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
0x200-0xFFF - Program ROM and work RAM
*/
#include "Chip8.h"
#include <fstream>
#include <string>
#include "windows.h"
#include <SDL.h>

using namespace std;

const unsigned char chip8_fontset[80] =
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8()
{
	delay_timer = 0;
	sound_timer = 0;
	I = 0;
	pc = 0x200;
	opcode = 0;
	sp = 0;

	for (int i = 0; i < sizeof(V); i++)
		V[i] = 0;

	for (int i = 0; i < sizeof(gfx); i++)
		gfx[i] = 0;

	for (int i = 0; i < sizeof(chip8_fontset); ++i)
		memory[i] = chip8_fontset[i];
}

Chip8::~Chip8()
{
}

void Chip8::load()
{
	FILE* pFile;

	long lSize = 0;
	char* buffer;
	size_t result;

	pFile = fopen("Pong.ch8", "rb");

	if (pFile == NULL) { fputs("File error: ", stderr); exit(1); }

	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);

	buffer = (char*)malloc(sizeof(char) * lSize); // Enough memory for file + \0
	if (buffer == NULL) { fputs("Memory error", stderr); exit(2); }

	result = fread(buffer, 1, lSize, pFile);
	if (result != lSize) { fputs("Reading error", stderr); exit(3); }

	for (int i = 0; i <= lSize; ++i)
	{
		memory[i + 512] = (uint8_t)buffer[i];
	}

	fclose(pFile);
	free(buffer);
}

void Chip8::emulateCycle() {
	//fetch
	opcode = memory[pc] << 8 | memory[pc + 1];
	//decode
	//execute
	execute();

	//update
}

void Chip8::execute() {
	switch (opcode & 0xF000)
	{
	case 0x0000:
		switch (opcode & 0x000F)
		{
		case 0x0000: //0x00E0
			//clear screen
			break;
		case 0x000E: //0x00EE
			//return from subroutine
			break;
		default:
			printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
			throw exception((char*)opcode);
		}
		break;

	case 0x6000:
		V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
		pc += 2;
		break;

	case 0xA000:
		I = opcode & 0x0FFF;
		pc += 2;
		break;

	case 0x2000:
		stack[sp] = pc;
		sp++;
		pc = opcode & 0x0FFF;
		break;

	case 0x8000:
		switch (opcode & 0x000F)
		{
		case 0x0004:
			if (V[opcode & 0x00F0 >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
				V[0xF] = 1; // carry
			else
				V[0xF] = 0;
			V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
		default:
			break;
		}

		//DXYN: Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. 
		//Each row of 8 pixels is read as bit-coded starting from memory location I;
		//I value doesn’t change after the execution of this instruction. 
		//As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn,
		//and to 0 if that doesn’t happen 
	case 0xD000: {
		unsigned short N = (opcode & 0x000F);
		unsigned short Vx = V[opcode & 0x0F00 >> 8];
		unsigned short Vy = V[opcode & 0x00F0 >> 4];
		unsigned short height = opcode & 0x000F;
		drawSprite(Vx, Vy, N, height);
		drawFlag = true;
		pc += 2;
	}
	default: {printf("unknown opcode 0x%X\n", opcode); }

	}
}

void Chip8::drawSprite(unsigned short Vx, unsigned short Vy, unsigned short N, unsigned short height) {
	unsigned short pixel;
	drawGraphics();
	for (size_t i = 0; i < height; i++)
	{

	}
}

void Chip8::drawGraphics() {
	SDL_CreateWindow("asdf", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
}

void Chip8::run() {
	for (;;)
	{
		// Emulate one cycle
		emulateCycle();

		// If the draw flag is set, update the screen
		if (drawFlag)
			drawGraphics();

		// Store key press state (Press and Release)
		//myChip8.setKeys();
	}
}
