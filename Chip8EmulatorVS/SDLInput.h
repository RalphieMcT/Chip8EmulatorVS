#pragma once
#include <vector>
#include "Input.h"

class SDLInput : public Input{
public:
    SDLInput();
    ~SDLInput();
    std::vector<bool> keys();
};

