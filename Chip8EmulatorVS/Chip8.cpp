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
#include <iostream>
#include <sstream>
#include <iomanip>
#include "windows.h"
#include <SDL.h>
#include <bitset>

std::vector<uint8_t> chip8_fontset =
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

Chip8::Chip8(Display* display)
{
	_display = display;
	delay_timer = 0;
	sound_timer = 0;
	I = 0;
	pc = 0x200;
	opcode = 0;
	sp = 0;
	memory = std::vector<uint8_t>(MEMORY_SIZE);
	V = std::vector<uint8_t>(REGISTER_COUNT);
	stack = std::vector<uint16_t>(STACK_SIZE);
	key = std::vector<bool>(16);
	resetGfx();
	std::fill(memory.begin(), memory.end(), 0x0);
	std::fill(V.begin(), V.end(), 0x0);
	std::fill(stack.begin(), stack.end(), 0x0);
	std::fill(key.begin(), key.end(), 0x0);
	for (int i = 0; i < 80; i++)
	{
		memory[i] = chip8_fontset[i];
	}
}

Chip8::~Chip8()
{
	//SDL_DestroyRenderer(renderer);
	//SDL_DestroyWindow(window);
}

void Chip8::resetGfx() {
	std::vector<std::vector<uint8_t>> tmpGfx;
	for (int y = 0; y < GFX_HEIGHT; y++)
	{
		std::vector<uint8_t> line;
		for (int x = 0; x < GFX_WIDTH; x++)
		{
			line.push_back(0);
		}
		tmpGfx.push_back(line);
	}
	gfx = tmpGfx;
	drawFlag = true;
}

void Chip8::load()
{
	FILE* pFile;

	long lSize = 0;
	char* buffer;
	size_t result;

	pFile = fopen("life.ch8", "rb");

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
	opcode = memory[pc] << 8 | memory[pc + 1];
	execute();
}

void Chip8::execute() {
	uint8_t msb = ((opcode & 0xF000) >> 12);
	uint8_t x = ((opcode & 0x0F00) >> 8);
	uint8_t y = ((opcode & 0x00F0) >> 4);
	uint8_t n = (opcode & 0x000F);
	uint8_t nn = (opcode & 0x00FF);
	uint16_t nnn = (opcode & 0x0FFF);

	switch (msb)
	{
	case 0x0:
		switch (nnn)
		{
		case 0xE0:
			resetGfx();
			break;
		case 0xEE:
			pc = stack[--sp];
			pc -= 2;
			break;
		default:
			printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
		}
		break;
	case 0x1:
		pc = nnn;
		pc -= 2;
		break;
	case 0x2:
		stack[sp++] = pc + 2;
		pc = nnn;
		pc -= 2;
		break;
	case 0x3:
		if (V[x] == nn)
			pc += 2;
		break;
	case 0x4:
		if (V[x] != nn) 
			pc += 2;
		break;
	case 0x5:
		if (V[x] == V[y])
		{
			pc += 2;
		}
		break;
	case 0x6:
		V[x] = nn;
		break;

	case 0xA:
		I = nnn;
		break;
	case 0xB:
		pc = nnn + V[0];
		break;
	case 0xC:
		V[x] = rand() & nn;
		break;
	case 0x7:
		V[x] += nn;
		break;
	case 0x9:
		if (V[x] != V[y])
			pc += 2;
		break;
	case 0x8:
		switch (n)
		{
		case 0x0:
			V[x] = V[y];
			break;
		case 0x1:
			V[x] |= V[y];
			break;
		case 0x2:
			V[x] &= V[y];
			break;
		case 0x3:
			V[x] ^= V[y];
			break;
		case 0x4:
			V[15] = 0;
			if (V[x] + V[y] <= 0x00FF) {
				V[x] += V[y];
			}
			else {
				V[x] += V[y];
				V[15] = 1;
			}
			break;
		case 0x5:
			V[15] = 0;
			if (V[y] > V[x]) {
				V[x] -= V[y];
			}
			else {
				V[x] -= V[y];
				V[15] = 1;
			}
			break;
		case 0x6:
			V[0xF] = V[x] & 0b1; // carry
			V[x] >>= 1;
			break;
		case 0x7:
			V[0xF] = 1;
			if (V[x] > V[y])
			{
				V[0xF] = 0;
			}
			V[x] = V[y] - V[x];
			break;
		case 0xE: {
			std::bitset<8> bs(V[x]);
			V[0xF] = bs[bs.size() - 1];
			V[x] <<= 1;
			break;
		}
		default:
			printf("unknown opcode 0x%X\n", opcode);
			break;
		}
		break;


	case 0xD: {
		unsigned short Vx = V[x];
		unsigned short Vy = V[y];
		drawSprite(Vx, Vy, n);
		break;
	}
	/* EX9E 	KeyOp 	if(key()==Vx) 	
	Skips the next instruction if the key stored in VX is pressed. 
	(Usually the next instruction is a jump to skip a code block)
	EXA1 	KeyOp 	if(key()!=Vx) 	
	Skips the next instruction if the key stored in VX isn't pressed. 
	(Usually the next instruction is a jump to skip a code block) */
	case 0xE:
		switch (nn)
		{
		case 0x9E: {
			if (key.at(V[x]) == true)
				pc += 2;
			break;
		}
		case 0xA1: {
			if (key.at(V[x]) == false)
				pc += 2;
			break;
		}
		default:
			printf("unknown opcode 0x%X\n", opcode);
			break;
		}
		break;
	case 0xF:
		switch (nn)
		{
		case 0x07:
			V[x] = delay_timer;
			break;
		case 0x15:
			delay_timer = V[x];
			break;
		case 0x18:
			sound_timer = V[x];
			break;
		case 0x29:
			I = V[x] * 0x5;
			break;
		case 0xA:{
			bool pressed = false;
			for (uint8_t i = 0; i < key.size(); i++)
			{
				if (key.at(i) > 0)
				{
					pressed = true;
					V[x] = i;
					break;
				}
			}
			if (pressed == false)
			{
				pc -= 2;
			}
		}
			break;
		case 0x33:
			memory[I] = (V[x] / 100);
			memory[I + 1] = ((V[x] / 10) % 10);
			memory[I + 2] = ((V[x] % 100) % 10);
			break;
		case 0x55:
			for (auto i = 0; i <= x; i++)
			{
				memory[I + i] = V[i];
			}
			break;
		case 0x65:
			for (auto i = 0; i <= x; i++)
			{
				V[i] = memory[I + i];
			}
			break;
		case 0x1E:
			I += V[x];
			break;
		default:
			printf("unknown opcode 0x%X\n", opcode);
			break;
		}
		break;
	}
	pc += 2;
}

//DXYN: Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. 
//Each row of 8 pixels is read as bit-coded starting from memory location I;
//I value doesn’t change after the execution of this instruction. 
//As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn,
//and to 0 if that doesn’t happen 
void Chip8::drawSprite(unsigned short Vx, unsigned short Vy, unsigned short height) {
	unsigned short pixel;

	V[0xF] = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			pixel = memory[I + y];
			if ((pixel & (0x80 >> x)) != 0)
			{
				if (gfx.at((Vy + y) % GFX_HEIGHT).at((Vx + x) % GFX_WIDTH) == 1)
				{
					V[0xF] = 1;
				}
				gfx.at((Vy + y) % GFX_HEIGHT).at((Vx + x) % GFX_WIDTH) ^= 0x1;
				drawFlag = true;
			}
		}
	}
}
/*
void Chip8::drawGraphics() {
	for (auto y = 0; y < GFX_HEIGHT; y++)
	{
		for (int x = 0; x < GFX_WIDTH; x++)
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
	drawFlag = false;
}*/
/*
void Chip8::clearScreen()
{
	SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
	SDL_RenderFillRect(renderer, &screen_rect);
	SDL_RenderPresent(renderer);
}
*/
void Chip8::setKeys() {
	SDL_PumpEvents();

	// update keyboard state
	//keysArray = const_cast <Uint8*> (SDL_GetKeyboardState(NULL));

	const Uint8* state = const_cast <Uint8*> (SDL_GetKeyboardState(NULL));
	key.at(0x0) = state[SDL_SCANCODE_1];
	key.at(0x1) = state[SDL_SCANCODE_2];
	key.at(0x2) = state[SDL_SCANCODE_3];
	key.at(0x3) = state[SDL_SCANCODE_4];
	key.at(0x4) = state[SDL_SCANCODE_Q];
	key.at(0x5) = state[SDL_SCANCODE_W];
	key.at(0x6) = state[SDL_SCANCODE_E];
	key.at(0x7) = state[SDL_SCANCODE_R];
	key.at(0x8) = state[SDL_SCANCODE_A];
	key.at(0x9) = state[SDL_SCANCODE_S];
	key.at(0xa) = state[SDL_SCANCODE_D];
	key.at(0xb) = state[SDL_SCANCODE_F];
	key.at(0xc) = state[SDL_SCANCODE_Z];
	key.at(0xd) = state[SDL_SCANCODE_X];
	key.at(0xe) = state[SDL_SCANCODE_C];
	key.at(0xf) = state[SDL_SCANCODE_V];
}

void Chip8::run() {
	for (;;)
	{
		// Emulate one cycle
		emulateCycle();

		// If the draw flag is set, update the screen
		if (drawFlag) {
			_display->drawGraphics(gfx);
			drawFlag = false;
		}

		// Store key press state (Press and Release)
		setKeys();
		if (delay_timer > 0)
			delay_timer--;

		if (sound_timer > 0)
			sound_timer--;
	}
}
