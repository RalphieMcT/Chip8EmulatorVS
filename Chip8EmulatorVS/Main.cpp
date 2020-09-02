//#pragma warning(suppress : 4996)
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include "Chip8.h"
#include "Main.h"
#include "WinDisplay.h"
#include "SDLInput.h"
#include <experimental/filesystem>
#include <string>
#include <iostream>
#include <sstream>
#include <conio.h>

using namespace std::experimental;

int main(int argc, char* argv[])
{
	std::string rom = "Pong.ch8";
	bool readroms = true;
	if (readroms)
	{
		rom = chooseRom();
		std::cout << rom << std::endl;
	}
	Display *display = new WinDisplay(64, 32);
	Input *input = new SDLInput();
	Chip8 *myChip8 = new Chip8(display, input);
	myChip8->load(rom.c_str());
	
	for (;;) {
		SDL_PumpEvents();
		const Uint8* state = const_cast <Uint8*> (SDL_GetKeyboardState(NULL));
		if (state[SDL_SCANCODE_SPACE])
		{
			myChip8->reset();
		}
		if (state[SDL_SCANCODE_ESCAPE])
		{
			break;
		}
		myChip8->run();
	}

	delete display;
	delete myChip8;
	delete input;
	
	return 0;
}

std::string chooseRom()
{
	std::vector<std::string> roms;
	std::string s = filesystem::current_path().u8string();
	system("CLS");
	std::cout << "Current ROM directory: " << s << std::endl;

	int i = 0;
	for (auto & f : filesystem::directory_iterator(s))
	{
		if (f.path().extension() == ".ch8")
		{
			std::cout << i << "=" << f.path().filename() << std::endl;
			i++;
			roms.push_back(f.path().filename().u8string());
		}
	}
	int romnum = -1;
	std::string input = "";
	char* p;
	while (true)
	{
		std::cin >> input;
		strtol(input.c_str(), &p, 10);
		if (*p==0)
		{
			romnum = atoi(input.c_str());
		}
		else
		{
			std::cout << "Not a number" << std::endl;
		}
		if (romnum < 0 || romnum >= (int)roms.size())
		{
			std::cout << "Invalid number" << std::endl;
			continue;
		}
		break;
	}
	std::string romname = roms.at(romnum);
	return romname;
}

