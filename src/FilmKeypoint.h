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
    LayerInteractDefault,
    LayerEnable,
    LayerDisable,
    LayerAwait,
    LayerRemove,
    OccupyInput,
    ReleaseInput
};

struct FilmKeypoint {
    using Duration = std::chrono::duration<float>;

    Duration delay;
    int frame_delay : 29;
    int need_input : 1;
    int need_parallel : 1;
    int need_await : 1;

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

struct FilmKeypointLayer : public FilmKeypoint {
    LayerIndex layerindx;
};

struct FilmKeypointLayerInteractPos : public FilmKeypointLayer {
    SDL_FRect pos = { 0.f };
    float (*ease_func)(float);

    FilmKeypointType type() const override { return FilmKeypointType::LayerInteractPos; }
};

struct FilmKeypointLayerInteractSourcePos : public FilmKeypointLayer {
    SDL_FRect src_pos = { 0.f };

    FilmKeypointType type() const override { return FilmKeypointType::LayerInteractSourcePos; }
};

struct FilmKeypointLayerInteractAlpha : public FilmKeypointLayer {
    uint8_t alpha;
    float (*ease_func)(float);

    FilmKeypointType type() const override { return FilmKeypointType::LayerInteractAlpha; }
};

struct FilmKeypointLayerInteractSwap : public FilmKeypointLayer {
    ResourceIndex texindx;

    FilmKeypointType type() const override { return FilmKeypointType::LayerInteractSwap; }
};

struct FilmKeypointLayerInteractTransparentSwap : public FilmKeypointLayer {
    ResourceIndex texindx;
    float (*ease_func)(float);

    FilmKeypointType type() const override { return FilmKeypointType::LayerInteractTransparentSwap; }
};

struct FilmKeypointLayerInteractDefault : public FilmKeypointLayer {
    FilmKeypointType type() const override { return FilmKeypointType::LayerInteractDefault; }
};

struct FilmKeypointLayerEnable : public FilmKeypointLayer {
    FilmKeypointType type() const override { return FilmKeypointType::LayerEnable; }
};

struct FilmKeypointLayerDisable : public FilmKeypointLayer {
    FilmKeypointType type() const override { return FilmKeypointType::LayerDisable; }
};

struct FilmKeypointLayerRemove : public FilmKeypointLayer {
    FilmKeypointType type() const override { return FilmKeypointType::LayerRemove; }
};

struct FilmKeypointOccupyInput : public FilmKeypoint {
    FilmKeypointType type() const override { return FilmKeypointType::OccupyInput; }
};

struct FilmKeypointReleaseInput : public FilmKeypoint {
    FilmKeypointType type() const override { return FilmKeypointType::ReleaseInput; }
};

