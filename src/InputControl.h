#pragma once
#include <SDL3/SDL.h>

// TODO: Invent key and analog input system (including mouse)
class InputControl {
public:
    void MapDefaultInput();
    void MapAliasInput();

    int Type();

    void Listen();

private:
    

};

