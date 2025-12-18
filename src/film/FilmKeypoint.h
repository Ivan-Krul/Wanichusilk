#pragma once
#include <stdint.h>
#include <chrono>
#include <memory>

#include "../define.h"
#include "../Clock.h"
#include "../TextureManager.h"

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

struct FilmKeypoint : public TimerStep {
    inline virtual FilmKeypointTypeStruct type() const { return { FilmKeypointChangeType::None, 0 }; }
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

    TextureIndex to = -1;
    RenderMode rend_mode = blank;
};

struct FilmKeypointBgSwap : public FilmKeypointBackground {
    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Background, Swap }; }
};

struct FilmKeypointEase {
    float (*ease_func)(float) = nullptr;
};

struct FilmKeypointBgTransparentSwap : public FilmKeypointBackground, public FilmKeypointEase {
    TextureIndex from;

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
        InteractAnimateSwap,
        InteractAnimateSwapLoop,
        InteractLoop,
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
    TextureIndex texind = -1;
    inline virtual LayerBuildType layertype() const { return Texture; }
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
    uint8_t alpha = 255;

    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractAlpha }; }
    inline bool has_ease() override { return true; }
};

struct FilmKeypointLayerSwap : public FilmKeypointLayer {
    TextureIndex texindx = -1;

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

struct FilmKeypointLayerInteractAnimateSwap : public FilmKeypointLayer {
    struct Flip {
        int duration_ms;
        LockerIndex new_tex_ind;
    };
    
    std::vector<Flip> flips;

    LockerIndex delay_after_frame;

    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractAnimateSwap }; }
};

struct FilmKeypointLayerInteractAnimateSwapLoop : public FilmKeypointLayerInteractAnimateSwap {
    inline FilmKeypointTypeStruct type() const override { return { FilmKeypointChangeType::Layer, InteractAnimateSwapLoop }; }
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