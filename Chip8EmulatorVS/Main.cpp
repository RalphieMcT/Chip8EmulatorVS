#pragma warning(suppress : 4996)
#include "Chip8.h";
#include "Main.h"
#include "WinDisplay.h"

using namespace std;

int main(int argc, char* argv[])
{
	Display *display = new WinDisplay(64, 32);
	Chip8 *myChip8 = new Chip8(display);
	myChip8->load();
	myChip8->run();

	delete display;
	delete myChip8;
	return 0;
}
