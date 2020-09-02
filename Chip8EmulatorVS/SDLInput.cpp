#include "SDLInput.h"
#include <SDL.h>
SDLInput::SDLInput()
{
}

SDLInput::~SDLInput()
{
}

std::vector<bool> SDLInput::keys()
{
	SDL_PumpEvents();

	// update keyboard state
	//keysArray = const_cast <Uint8*> (SDL_GetKeyboardState(NULL));
	std::vector<bool> key = std::vector<bool>(16);
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
	return key;
}
