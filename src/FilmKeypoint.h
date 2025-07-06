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
    LayerInteractPartPos,
    LayerInteractDefaultPos,
    LayerInteractDefaultPartPos,
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

constexpr int cLayerIndexBegin = (int)(FilmKeypointType::LayerAdd);
constexpr int cLayerIndexEnd = (int)(FilmKeypointType::LayerRemove);

struct FilmTimer {
    using Duration = std::chrono::duration<float>;

    Duration delay;
    int frame_delay : 28;
    int need_time_delay : 1;
    int need_input : 1;
    int need_parallel : 1;
    int need_await : 1;

    inline FilmTimer() : delay(std::chrono::seconds(0)), frame_delay(0), need_time_delay(0), need_input(0), need_parallel(0), need_await(0) {}

    inline bool is_zero() const { return delay <= delay.zero() && frame_delay <= 0; }
    inline void decrement_frame(int diff_frame) { frame_delay--; }
    inline void decrement_time(Duration diff) { delay -= diff; }
    inline void decrement_time_frame(Duration diff) { need_time_delay ? (delay -= diff, frame_delay--) : (frame_delay--); }
};

struct FilmKeypoint : public FilmTimer {
    virtual FilmKeypointType type() const { return FilmKeypointType::BlankDelay; }
    virtual bool has_ease() const { return false; }
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

struct FilmKeypointEase {
    float (*ease_func)(float) = nullptr;

    bool has_ease() const { return ease_func != nullptr; }
};

struct FilmKeypointEaseTransparentSwap : public FilmKeypoint, public FilmKeypointEase {
    ResourceIndex to;
    ResourceIndex from;

    FilmKeypointType type() const override { return FilmKeypointType::EaseTransparentSwap; }
};

struct FilmKeypointLayerAdd : public FilmKeypoint {
    ResourceIndex texind;

    FilmKeypointType type() const override { return FilmKeypointType::LayerAdd; }
};

struct FilmKeypointLayer : public FilmKeypoint {
    LayerIndex layerindx;
};

struct FilmKeypointLayerInteractRect : public FilmKeypointLayer, public FilmKeypointEase {
    SDL_FRect rect = { 0.f };
};

struct FilmKeypointLayerInteractPos : public FilmKeypointLayerInteractRect {
    FilmKeypointType type() const override { return FilmKeypointType::LayerInteractPos; }
};

struct FilmKeypointLayerInteractPartitionPos : public FilmKeypointLayerInteractRect {
    FilmKeypointType type() const override { return FilmKeypointType::LayerInteractPartPos; }
};

struct FilmKeypointLayerDefaultInteractPos : public FilmKeypointLayerInteractRect {
    FilmKeypointType type() const override { return FilmKeypointType::LayerInteractDefaultPos; }
};

struct FilmKeypointLayerInteractDefaultPartitionPos : public FilmKeypointLayerInteractRect {
    FilmKeypointType type() const override { return FilmKeypointType::LayerInteractDefaultPartPos; }
};

struct FilmKeypointLayerInteractAlpha : public FilmKeypointLayer, public FilmKeypointEase {
    uint8_t alpha;

    FilmKeypointType type() const override { return FilmKeypointType::LayerInteractAlpha; }
};

struct FilmKeypointLayerInteractSwap : public FilmKeypointLayer {
    ResourceIndex texindx;

    FilmKeypointType type() const override { return FilmKeypointType::LayerInteractSwap; }
};

struct FilmKeypointLayerInteractTransparentSwap : public FilmKeypointLayer, public FilmKeypointEase {
    ResourceIndex texindx;

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


#ifdef DEBUG
static const char* debugKeypointNames[] = {
    "BlankDelay",
    "Swap",
    "TransparentSwap",
    "EaseTransparentSwap",
    "LayerAdd",
    "LayerInteractPos",
    "LayerInteractPartPos",
    "LayerInteractDefaultPos",
    "LayerInteractDefaultPartPos",
    "LayerInteractAlpha",
    "LayerInteractSwap",
    "LayerInteractTransparentSwap",
    "LayerInteractDefault",
    "LayerEnable",
    "LayerDisable",
    "LayerAwait",
    "LayerRemove",
    "OccupyInput",
    "ReleaseInput"
};
#endif
