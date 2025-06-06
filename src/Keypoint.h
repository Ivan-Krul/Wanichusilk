#pragma once
#include <stdint.h>
#include <chrono>

enum class FilmKeypointType {
    Swap
};

struct FilmKeypoint {
    size_t from;
    size_t to;

    union NextCondition {
        std::chrono::duration<float> delay;
        size_t need_input = 0;
        
    } next_condition;

    virtual FilmKeypointType type() const = 0;
};

struct FilmKeypointSwap : public FilmKeypoint {
    FilmKeypointType type() const override { return FilmKeypointType::Swap; }
};

//using FilmKeypointSwap = FilmKeypoint;


