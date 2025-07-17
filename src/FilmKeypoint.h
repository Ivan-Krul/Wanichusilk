#pragma once
#include <stdint.h>
#include <chrono>

#include "TextureManager.h"

typedef int LayerIndex;

enum class FilmKeypointChangeType : short {
    None = 0,
    Background,
    Layer
};

struct FilmKeypointTypeStruct {
    FilmKeypointChangeType global_type : 3;
    short specific_type : 13;
};

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
    virtual FilmKeypointTypeStruct type() const { return { 0 }; }
    virtual bool has_ease() const { return false; }
};

struct FilmKeypointBackground : public FilmKeypoint {
    enum Type : short {
        Swap = 1,
        TransparentSwap,
        EaseTransparentSwap
    };

    ResourceIndex to;
};

struct FilmKeypointBgSwap : public FilmKeypointBackground {
    FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Background, Swap }; }
};

struct FilmKeypointBgTransparentSwap : public FilmKeypointBackground {
    ResourceIndex from;

    FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Background, TransparentSwap }; }
};

struct FilmKeypointEase {
    float (*ease_func)(float) = nullptr;

    bool has_ease() const { return ease_func != nullptr; }
};

struct FilmKeypointEaseTransparentSwap : public FilmKeypointBackground, public FilmKeypointEase {
    ResourceIndex from;

    FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Background, EaseTransparentSwap }; }
};

struct FilmKeypointLayer : public FilmKeypoint {
    enum Type : short {
        Add = 1,
        InteractPos,
        InteractPartPos,
        InteractDefaultPos,
        InteractDefaultPartPos,
        InteractAlpha,
        InteractSwap,
        InteractTransparentSwap,
        InteractDefault,
        Enable,
        Disable,
        Await,
        Remove
    };
    LayerIndex layerindx;
};

struct FilmKeypointLayerAdd : public FilmKeypointLayer {
    ResourceIndex texind;

    FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, Add }; }
};

struct FilmKeypointLayerInteractRect : public FilmKeypointLayer, public FilmKeypointEase {
    SDL_FRect rect = { 0.f };
};

struct FilmKeypointLayerInteractPos : public FilmKeypointLayerInteractRect {
    FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractPos }; }
};

struct FilmKeypointLayerInteractPartitionPos : public FilmKeypointLayerInteractRect {
    FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractPartPos }; }
};

struct FilmKeypointLayerDefaultInteractPos : public FilmKeypointLayerInteractRect {
    FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractDefaultPos }; }
};

struct FilmKeypointLayerInteractDefaultPartitionPos : public FilmKeypointLayerInteractRect {
    FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractDefaultPartPos }; }
};

struct FilmKeypointLayerInteractAlpha : public FilmKeypointLayer, public FilmKeypointEase {
    uint8_t alpha;

    FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractAlpha }; }
};

struct FilmKeypointLayerInteractSwap : public FilmKeypointLayer {
    ResourceIndex texindx;

    FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractSwap }; }
};

struct FilmKeypointLayerInteractTransparentSwap : public FilmKeypointLayer, public FilmKeypointEase {
    ResourceIndex texindx;

    FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractTransparentSwap }; }
};

struct FilmKeypointLayerInteractDefault : public FilmKeypointLayer {
    FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractDefault }; }
};

struct FilmKeypointLayerEnable : public FilmKeypointLayer {
    FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, Enable }; }
};

struct FilmKeypointLayerDisable : public FilmKeypointLayer {
    FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, Disable }; }
};

struct FilmKeypointLayerRemove : public FilmKeypointLayer {
    FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, Remove }; }
};

struct FilmKeypointOccupyInput : public FilmKeypoint {
    FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::None, 1 }; }
};

struct FilmKeypointReleaseInput : public FilmKeypoint {
    FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::None, 2 }; }
};
