#include <vector>
#pragma once
class Display
{
	public:
		virtual ~Display();
		virtual void drawGraphics(std::vector<std::vector<uint8_t>> gfx) {

		}
};

