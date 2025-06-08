#pragma once
#include <stdint.h>
#include <chrono>

#include "TextureManager.h"

enum class FilmKeypointType {
    BlankDelay,
    Swap
};

struct FilmKeypoint {
    std::chrono::duration<float> delay;
    int frame_delay : 31;
    int need_input : 1;

    virtual FilmKeypointType type() const { return FilmKeypointType::BlankDelay; }
};

struct FilmKeypointSwap : public FilmKeypoint {
    ResourceIndex to;

    FilmKeypointType type() const override { return FilmKeypointType::Swap; }
};

