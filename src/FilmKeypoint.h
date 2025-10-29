#pragma once
#include <stdint.h>
#include <chrono>
#include <memory>

#include "TextureManager.h"
#include "define.h"

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

    enum ActionConcurency : unsigned int {
        Instant = 0,
        First,
        Await,
        InInputOrAwait,
        InInputOrFirst,
        InInputAfterAwait,
        InInputAfterFirst,
    };

    Duration delay;
    int frame_delay : 28;
    int need_time_delay : 1;
    ActionConcurency action : 3;

    inline FilmTimer() : delay(std::chrono::seconds(0)), frame_delay(0), need_time_delay(0), action(Instant) {}

    inline bool is_zero() const { return delay <= delay.zero() && frame_delay <= 0; }
    inline void set_delay_frame(int frames) { frame_delay = frames; need_time_delay = false; }
    inline void set_delay_time(Duration dur) { delay = dur; need_time_delay = true; }
    inline void decrement_frame(int diff_frame) { frame_delay--; }
    inline void decrement_time(Duration diff) { delay -= diff; }
    inline void decrement_time_frame(Duration diff) { need_time_delay ? (delay -= diff, frame_delay--) : (frame_delay--); }
};

struct FilmKeypoint : public FilmTimer {
    inline virtual FilmKeypointTypeStruct type() const { return { 0 }; }
    inline virtual bool has_ease() { return false; }
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

    ResourceIndex to = -1;
    RenderMode rend_mode = blank;
};

struct FilmKeypointBgSwap : public FilmKeypointBackground {
    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Background, Swap }; }
};

struct FilmKeypointEase {
    float (*ease_func)(float) = nullptr;
};

struct FilmKeypointBgTransparentSwap : public FilmKeypointBackground, public FilmKeypointEase {
    ResourceIndex from;

    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Background, TransparentSwap }; }
    inline bool has_ease() override { return true; }
};

struct FilmKeypointLayer : public FilmKeypoint {
    enum Type : short {
        Add = 1,
        InteractPos,
        InteractRectPos,
        InteractPartPos,
        InteractDefaultPos,
        InteractDefaultPartPos,
        InteractAlpha,
        InteractSwap,
        InteractTransparentSwap,
        InteractDefault,
        Enable,
        Disable,
        Await, // maybe unused
        Remove
    };
    LayerIndex layerindx = -1;
    virtual inline FilmKeypointTypeStruct type() const { return { FilmKeypointChangeType::Layer, 0 }; }
};

struct FilmKeypointLayerAdd : public FilmKeypointLayer {
    enum LayerBuildType : char {
        None = 0,
        Texture
    };

    inline virtual LayerBuildType layertype() const { return { None }; }
    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, Add }; }
};

struct FilmKeypointLayerAddTexture : public FilmKeypointLayerAdd {
    ResourceIndex texind;
    inline virtual LayerBuildType layertype() const { return { Texture }; }
};

struct FilmKeypointLayerInteractRect : public FilmKeypointLayer, public FilmKeypointEase {
    SDL_FRect rect = { 0.f };
    inline bool has_ease() override { return true; }
};

struct FilmKeypointLayerInteractPos : public FilmKeypointLayerInteractRect {
    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractPos }; }
};

struct FilmKeypointLayerInteractRectPos : public FilmKeypointLayerInteractRect {
    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractRectPos }; }
};

struct FilmKeypointLayerInteractPartitionPos : public FilmKeypointLayerInteractRect {
    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractPartPos }; }
};

struct FilmKeypointLayerDefaultInteractPos : public FilmKeypointLayerInteractRect {
    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractDefaultPos }; }
};

struct FilmKeypointLayerInteractDefaultPos : public FilmKeypointLayerInteractRect {
    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractDefaultPos }; }
};

struct FilmKeypointLayerInteractDefaultPartitionPos : public FilmKeypointLayerInteractRect {
    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractDefaultPartPos }; }
};

struct FilmKeypointLayerInteractAlpha : public FilmKeypointLayer, public FilmKeypointEase {
    uint8_t alpha;

    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractAlpha }; }
    inline bool has_ease() override { return true; }
};

struct FilmKeypointLayerSwap : public FilmKeypointLayer {
    ResourceIndex texindx = -1;

    enum SwapMode : short {
        KeepNotDeformed,
        SetDefault,
        NewTransform,
        KeepInAspect
    };

    SwapMode swap = KeepInAspect;
    std::unique_ptr<SDL_FRect> swap_rect_ptr = nullptr;
    std::unique_ptr<SDL_FRect> swap_part_ptr = nullptr;

    FilmKeypointLayerSwap() = default;
    FilmKeypointLayerSwap(const FilmKeypointLayerSwap& other)
        : FilmKeypointLayer(other),
        texindx(other.texindx),
        swap(other.swap),
        swap_rect_ptr(other.swap_rect_ptr ? std::make_unique<SDL_FRect>(*other.swap_rect_ptr) : nullptr),
        swap_part_ptr(other.swap_part_ptr ? std::make_unique<SDL_FRect>(*other.swap_part_ptr) : nullptr) {}
    FilmKeypointLayerSwap& operator=(const FilmKeypointLayerSwap& other);
    FilmKeypointLayerSwap(FilmKeypointLayerSwap&&) noexcept = default;
    FilmKeypointLayerSwap& operator=(FilmKeypointLayerSwap&&) noexcept = default;
};

struct FilmKeypointLayerInteractSwap : public FilmKeypointLayerSwap {
    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractSwap }; }
};

struct FilmKeypointLayerInteractTransparentSwap : public FilmKeypointLayerSwap, public FilmKeypointEase {
    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractTransparentSwap }; }
    inline bool has_ease() override { return true; }
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

//inline FilmKeypointLayerSwap::FilmKeypointLayerSwap(const FilmKeypointLayerSwap& other) {
//    texindx = other.texindx;
//    swap = other.swap;
//    if (other.swap_rect_ptr)
//        swap_rect_ptr = std::make_unique<SDL_FRect>(*other.swap_rect_ptr);
//    if (other.swap_part_ptr)
//        swap_part_ptr = std::make_unique<SDL_FRect>(*other.swap_part_ptr);
//}

inline FilmKeypointLayerSwap& FilmKeypointLayerSwap::operator=(const FilmKeypointLayerSwap& other) {
    if (this == &other) return *this;
    FilmKeypointLayer::operator=(other);
    texindx = other.texindx;
    swap = other.swap;
    swap_rect_ptr = other.swap_rect_ptr ? std::make_unique<SDL_FRect>(*other.swap_rect_ptr) : nullptr;
    swap_part_ptr = other.swap_part_ptr ? std::make_unique<SDL_FRect>(*other.swap_part_ptr) : nullptr;
    return *this;
}

namespace FilmKP {
    inline FilmTimer min(const FilmTimer a, const FilmTimer b) {
        FilmTimer timer;
        timer.frame_delay = std::min<>(a.frame_delay, b.frame_delay);
        timer.delay = std::min<>(a.delay, b.delay);
        if (timer.delay != timer.delay.zero()) timer.need_time_delay = true;
        return timer;
    }

    inline FilmTimer max(const FilmTimer a, const FilmTimer b) {
        FilmTimer timer;
        timer.frame_delay = std::max<>(a.frame_delay, b.frame_delay);
        timer.delay = std::max<>(a.delay, b.delay);
        if (timer.delay != timer.delay.zero()) timer.need_time_delay = true;
        return timer;
    }

#ifdef DEBUG
    inline void SDL_Log_FilmTimer(const FilmTimer other) {
        switch (other.action) {
        case FilmTimer::Instant: SDL_Log("FilmTimer: %fs or %d frames with Instant", other.delay.count(), other.frame_delay); break;
        case FilmTimer::First: SDL_Log("FilmTimer: %fs or %d frames with First", other.delay.count(), other.frame_delay); break;
        case FilmTimer::Await: SDL_Log("FilmTimer: %fs or %d frames with Await", other.delay.count(), other.frame_delay); break;
        case FilmTimer::InInputOrAwait: SDL_Log("FilmTimer: %fs or %d frames with InInputOrAwait", other.delay.count(), other.frame_delay); break;
        case FilmTimer::InInputOrFirst: SDL_Log("FilmTimer: %fs or %d frames with InInputOrFirst", other.delay.count(), other.frame_delay); break;
        case FilmTimer::InInputAfterAwait: SDL_Log("FilmTimer: %fs or %d frames with InInputAfterAwait", other.delay.count(), other.frame_delay); break;
        case FilmTimer::InInputAfterFirst: SDL_Log("FilmTimer: %fs or %d frames with InInputAfterFirst", other.delay.count(), other.frame_delay); break;   
        }
    }
#else // inlining nothing should be enough to not generate code for logging in release
    inline void SDL_Log_FilmTimer(const FilmTimer other) {}
#endif
}

/*
    using KPTypeStruct = FilmKeypointTypeStruct;
    using KPBackground = FilmKeypointBackground;
    using KPBgSwap = FilmKeypointBgSwap;
    using KPEase = FilmKeypointEase;
    using KPBgTransparentSwap = FilmKeypointBgTransparentSwap;
    using KPLayer = FilmKeypointLayer;
    using KPLayerAdd = FilmKeypointLayerAdd;
    using KPLayerInteractRect = FilmKeypointLayerInteractRect;
    using KPLayerInteractPos = FilmKeypointLayerInteractPos;
    using KPLayerInteractRectPos = FilmKeypointLayerInteractRectPos;
    using KPLayerInteractPartitionPos = FilmKeypointLayerInteractPartitionPos;
    using KPLayerDefaultInteractPos = FilmKeypointLayerDefaultInteractPos;
    using KPLayerInteractDefaultPartitionPos = FilmKeypointLayerInteractDefaultPartitionPos;
    using KPLayerInteractAlpha = FilmKeypointLayerInteractAlpha;
    using KPLayerSwap = FilmKeypointLayerSwap;
    using KPLayerInteractSwap = FilmKeypointLayerInteractSwap;
    using KPLayerInteractTransparentSwap = FilmKeypointLayerInteractTransparentSwap;
    using KPLayerInteractDefault = FilmKeypointLayerInteractDefault;
    using KPLayerEnable = FilmKeypointLayerEnable;
    using KPLayerDisable = FilmKeypointLayerDisable;
    using KPLayerRemove = FilmKeypointLayerRemove;
    using KPOccupyInput = FilmKeypointOccupyInput;
    using KPReleaseInput = FilmKeypointReleaseInput;
}
*/