#pragma warning(suppress : 4996)
#include "Chip8.h";
#include "Main.h"


using namespace std;

int main()
{
	Chip8 myChip8;

	myChip8.load();

	myChip8.run();
	//myChip8.

	cout << myChip8.V << endl;
	return 0;
}
