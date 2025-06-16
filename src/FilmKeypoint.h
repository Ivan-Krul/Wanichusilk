#pragma once
#include <stdint.h>
#include <chrono>

#include "TextureManager.h"

typedef int LayerIndex;

enum class FilmKeypointType {
    BlankDelay = 0,
    Swap,
    TransparentSwap,
    EaseTransparentSwap,
    LayerAdd,
    LayerInteractPos,
    LayerInteractSourcePos,
    LayerInteractAlpha,
    LayerInteractSwap,
    LayerInteractTransparentSwap,
    LayerEnable,
    LayerDisable,
    LayerRemove,
    OccupyInput,
    ReleaseInput
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

struct FilmKeypointTransparentSwap : public FilmKeypoint {
    ResourceIndex to;
    ResourceIndex from;

    FilmKeypointType type() const override { return FilmKeypointType::TransparentSwap; }
};

struct FilmKeypointEaseTransparentSwap : public FilmKeypoint {
    ResourceIndex to;
    ResourceIndex from;
    float (*ease_func)(float);

    FilmKeypointType type() const override { return FilmKeypointType::EaseTransparentSwap; }
};

struct FilmKeypointLayerAdd : public FilmKeypoint {
    ResourceIndex texind;

    FilmKeypointType type() const override { return FilmKeypointType::LayerAdd; }
};

struct FilmKeypointLayerInteractPos : public FilmKeypoint {
    LayerIndex layerindx;
    SDL_FRect pos = { 0.f };
    float (*ease_func)(float);

    FilmKeypointType type() const override { return FilmKeypointType::LayerInteractPos; }
};

struct FilmKeypointLayerInteractSourcePos : public FilmKeypoint {
    LayerIndex layerindx;
    SDL_FRect src_pos = { 0.f };
    float (*ease_func)(float);

    FilmKeypointType type() const override { return FilmKeypointType::LayerInteractSourcePos; }
};

struct FilmKeypointLayerInteractAlpha : public FilmKeypoint {
    LayerIndex layerindx;
    uint8_t alpha;
    float (*ease_func)(float);

    FilmKeypointType type() const override { return FilmKeypointType::LayerInteractAlpha; }
};

struct FilmKeypointLayerInteractSwap : public FilmKeypoint {
    ResourceIndex texindx;

    FilmKeypointType type() const override { return FilmKeypointType::LayerInteractSwap; }
};

struct FilmKeypointLayerInteractTransparentSwap : public FilmKeypoint {
    ResourceIndex texindx;
    float (*ease_func)(float);

    FilmKeypointType type() const override { return FilmKeypointType::LayerInteractTransparentSwap; }
};

struct FilmKeypointLayerEnable : public FilmKeypoint {
    LayerIndex layerindx;

    FilmKeypointType type() const override { return FilmKeypointType::LayerEnable; }
};

struct FilmKeypointLayerDisable : public FilmKeypoint {
    LayerIndex layerindx;

    FilmKeypointType type() const override { return FilmKeypointType::LayerDisable; }
};

struct FilmKeypointLayerRemove : public FilmKeypoint {
    LayerIndex layerindx;

    FilmKeypointType type() const override { return FilmKeypointType::LayerRemove; }
};

struct FilmKeypointOccupyInput : public FilmKeypoint {
    FilmKeypointType type() const override { return FilmKeypointType::OccupyInput; }
};

struct FilmKeypointReleaseInput : public FilmKeypoint {
    FilmKeypointType type() const override { return FilmKeypointType::ReleaseInput; }
};

