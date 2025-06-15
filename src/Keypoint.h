#pragma once
#include <stdint.h>
#include <chrono>

#include "TextureManager.h"

enum class FilmKeypointType {
    BlankDelay,
    Swap,
    TransparentSwap
};

struct FilmKeypoint {
    using Duration = std::chrono::duration<float>;

    Duration delay;
    int frame_delay : 31;
    int need_input : 1;

    virtual FilmKeypointType type() const { return FilmKeypointType::BlankDelay; }
};

struct FilmKeypointSwap : public FilmKeypoint {
    ResourceIndex to;

    FilmKeypointType type() const override { return FilmKeypointType::Swap; }
};

struct FilmKeypointTransparentSwap : public FilmKeypointSwap {
    ResourceIndex from;

    FilmKeypointType type() const override { return FilmKeypointType::TransparentSwap; }
};


