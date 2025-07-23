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
    inline void set_delay_frame(int frames) { frame_delay = frames; need_time_delay = false; }
    inline void set_delay_time(Duration dur) { delay = dur; need_time_delay = true; }
    inline void decrement_frame(int diff_frame) { frame_delay--; }
    inline void decrement_time(Duration diff) { delay -= diff; }
    inline void decrement_time_frame(Duration diff) { need_time_delay ? (delay -= diff, frame_delay--) : (frame_delay--); }
};

struct FilmKeypoint : public FilmTimer {
    inline virtual FilmKeypointTypeStruct type() const { return { 0 }; }
    inline virtual bool has_ease() const { return false; }
};

struct FilmKeypointBackground : public FilmKeypoint {
    enum Type : short {
        Swap = 1,
        TransparentSwap
    };

    enum RenderMode {
        blank,
        simple,
        centered_black_borders,
        //centered_fill,
        //scroll
    };

    ResourceIndex to;
    RenderMode rend_mode;
};

struct FilmKeypointBgSwap : public FilmKeypointBackground {
    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Background, Swap }; }
};

struct FilmKeypointEase {
    float (*ease_func)(float) = nullptr;

    inline bool has_ease() const { return ease_func != nullptr; }
};

struct FilmKeypointBgTransparentSwap : public FilmKeypointBackground, public FilmKeypointEase {
    ResourceIndex from;

    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Background, TransparentSwap }; }
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

    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, Add }; }
};

struct FilmKeypointLayerInteractRect : public FilmKeypointLayer, public FilmKeypointEase {
    SDL_FRect rect = { 0.f };
};

struct FilmKeypointLayerInteractPos : public FilmKeypointLayerInteractRect {
    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractPos }; }
};

struct FilmKeypointLayerInteractPartitionPos : public FilmKeypointLayerInteractRect {
    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractPartPos }; }
};

struct FilmKeypointLayerDefaultInteractPos : public FilmKeypointLayerInteractRect {
    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractDefaultPos }; }
};

struct FilmKeypointLayerInteractDefaultPartitionPos : public FilmKeypointLayerInteractRect {
    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractDefaultPartPos }; }
};

struct FilmKeypointLayerInteractAlpha : public FilmKeypointLayer, public FilmKeypointEase {
    uint8_t alpha;

    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractAlpha }; }
};

struct FilmKeypointLayerInteractSwap : public FilmKeypointLayer {
    ResourceIndex texindx;

    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractSwap }; }
};

struct FilmKeypointLayerInteractTransparentSwap : public FilmKeypointLayer, public FilmKeypointEase {
    ResourceIndex texindx;

    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractTransparentSwap }; }
};

struct FilmKeypointLayerInteractDefault : public FilmKeypointLayer {
    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractDefault }; }
};

struct FilmKeypointLayerEnable : public FilmKeypointLayer {
    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, Enable }; }
};

struct FilmKeypointLayerDisable : public FilmKeypointLayer {
    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, Disable }; }
};

struct FilmKeypointLayerRemove : public FilmKeypointLayer {
    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, Remove }; }
};

struct FilmKeypointOccupyInput : public FilmKeypoint {
    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::None, 1 }; }
};

struct FilmKeypointReleaseInput : public FilmKeypoint {
    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::None, 2 }; }
};
