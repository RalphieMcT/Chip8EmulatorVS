#pragma once
#include <vector>

class Input
{
public:
    virtual ~Input();
    virtual std::vector<bool> keys();
};

