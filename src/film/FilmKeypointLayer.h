#pragma once
#include "FilmKeypoint.h"

namespace film {
    struct KeypointLayer : public Keypoint {
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
            InteractAnimationStart,
            InteractAnimationStop,
            InteractAnimationLoop,
            InteractAnimationUnloop,
            InteractAnimationSpeed,
            InteractDefault,
            Enable,
            Disable,
            Await, // maybe unused
            Remove
        };
        LayerIndex layerindx = -1;
        virtual inline KeypointTypeStruct type() const { return { KeypointChangeType::Layer, 0 }; }
    };

    struct KeypointLayerAdd : public KeypointLayer {
        enum LayerBuildType : char {
            None = 0,
            Texture,
            Animation
        };

        inline virtual LayerBuildType layertype() const { return { None }; }
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, Add }; }
    };

    struct KeypointLayerAddTexture : public KeypointLayerAdd {
        TextureIndex texind = -1;
        inline virtual LayerBuildType layertype() const { return Texture; }
    };

    struct KeypointLayerInteractRect : public KeypointLayer, public KeypointEase {
        SDL_FRect rect = { 0.f };
        inline bool has_ease() override { return true; }
    };

    struct KeypointLayerInteractPos : public KeypointLayerInteractRect {
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, InteractPos }; }
    };

    struct KeypointLayerInteractRectPos : public KeypointLayerInteractRect {
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, InteractRectPos }; }
    };

    struct KeypointLayerInteractPartitionPos : public KeypointLayerInteractRect {
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, InteractPartPos }; }
    };

    struct KeypointLayerDefaultInteractPos : public KeypointLayerInteractRect {
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, InteractDefaultPos }; }
    };

    struct KeypointLayerInteractDefaultPos : public KeypointLayerInteractRect {
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, InteractDefaultPos }; }
    };

    struct KeypointLayerInteractDefaultPartitionPos : public KeypointLayerInteractRect {
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, InteractDefaultPartPos }; }
    };

    struct KeypointLayerInteractAlpha : public KeypointLayer, public KeypointEase {
        uint8_t alpha = 255;

        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, InteractAlpha }; }
        inline bool has_ease() override { return true; }
    };

    struct KeypointLayerSwap : public KeypointLayer {
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

        KeypointLayerSwap() = default;
        KeypointLayerSwap(const KeypointLayerSwap& other)
            : KeypointLayer(other),
            texindx(other.texindx),
            swap(other.swap),
            swap_rect_ptr(other.swap_rect_ptr ? std::make_unique<SDL_FRect>(*other.swap_rect_ptr) : nullptr),
            swap_part_ptr(other.swap_part_ptr ? std::make_unique<SDL_FRect>(*other.swap_part_ptr) : nullptr) {
        }
        KeypointLayerSwap& operator=(const KeypointLayerSwap& other);
        KeypointLayerSwap(KeypointLayerSwap&&) noexcept = default;
        KeypointLayerSwap& operator=(KeypointLayerSwap&&) noexcept = default;
    };

    struct KeypointLayerInteractSwap : public KeypointLayerSwap {
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, InteractSwap }; }
    };

    struct KeypointLayerInteractTransparentSwap : public KeypointLayerSwap, public KeypointEase {
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, InteractTransparentSwap }; }
        inline bool has_ease() override { return true; }
    };

    struct KeypointLayerAddAnimation : public KeypointLayerAdd {
        LockerIndex animind = -1;
        inline virtual LayerBuildType layertype() const { return Animation; }
    };

    struct KeypointLayerInteractAnimationSpeed : public KeypointLayer {
        float speed = 1.f;
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, InteractAnimationSpeed }; }
    };

    struct KeypointLayerInteractAnimationLoop : public KeypointLayer {
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, InteractAnimationLoop }; }
    };

    struct KeypointLayerInteractAnimationUnloop : public KeypointLayer {
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, InteractAnimationUnloop }; }
    };

    struct KeypointLayerInteractAnimationStart : public KeypointLayer {
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, InteractAnimationStart }; }
    };

    struct KeypointLayerInteractAnimationStop : public KeypointLayer {
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, InteractAnimationStop }; }
    };

    struct KeypointLayerInteractDefault : public KeypointLayer {
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, InteractDefault }; }
    };

    struct KeypointLayerEnable : public KeypointLayer {
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, Enable }; }
    };

    struct KeypointLayerDisable : public KeypointLayer {
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, Disable }; }
    };

    struct KeypointLayerRemove : public KeypointLayer {
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, Remove }; }
    };

    struct KeypointOccupyInput : public Keypoint {
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::None, 1 }; }
    };

    struct KeypointReleaseInput : public Keypoint {
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::None, 2 }; }
    };

    inline KeypointLayerSwap& KeypointLayerSwap::operator=(const KeypointLayerSwap& other) {
        if (this == &other) return *this;
        KeypointLayer::operator=(other);
        texindx = other.texindx;
        swap = other.swap;
        swap_rect_ptr = other.swap_rect_ptr ? std::make_unique<SDL_FRect>(*other.swap_rect_ptr) : nullptr;
        swap_part_ptr = other.swap_part_ptr ? std::make_unique<SDL_FRect>(*other.swap_part_ptr) : nullptr;
        return *this;
    }
}
