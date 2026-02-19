#pragma once
#include "FilmKeypoint.h"

namespace film {
    struct KeypointLayer : public Keypoint {
        enum Type : short {
            Add = 1,

            GroupJoin,
            GroupSharedInteract,
            GroupInteract,
            GroupRemove,

            SpriteJoin,
            SpriteSwap,
            SpriteSharedInteract,
            SpriteInteract,
            SpriteRemove,

            InteractPos,
            InteractRectPos,
            InteractPartPos,
            InteractDefaultPos,
            InteractDefaultPartPos,
            InteractAlpha,
            InteractSwap,
            InteractColor,

            InteractAnimationStart,
            InteractAnimationStop,
            InteractAnimationLoop,
            InteractAnimationUnloop,
            InteractAnimationSpeed,

            InteractTextWrap,
            InteractTextNew,
            InteractTextAppend,
            InteractTextInsert,
            InteractTextSwapFont,

            InteractDefault,
            Enable,
            Disable,
            Remove
        };
        LayerIndex layerindx = -1;
        virtual inline KeypointTypeStruct type() const { return { KeypointChangeType::Layer, 0 }; }
    };

    struct KeypointLayerAdd : public KeypointLayer {
        enum LayerBuildType : char {
            None = 0,
            Group,
            Sprite,
            Texture,
            Animation,
            Text
        };

        LockerIndex loaderind = -1;

        inline virtual LayerBuildType layertype() const { return { None }; }
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, Add }; }
    };

    struct KeypointLayerAddGroup : public KeypointLayerAdd {
        inline virtual LayerBuildType layertype() const { return Group; }
    };

    struct KeypointLayerGroupJoin : public KeypointLayer {
        LockerIndex joining_layerindx = -1;
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, GroupJoin }; }
    };

    struct KeypointLayerGroupSharedInteract : public KeypointLayer {
        std::shared_ptr<KeypointLayer> keypoint;
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, GroupSharedInteract }; }
    };

    struct KeypointLayerGroupInteract : public KeypointLayer {
        LockerIndex sprite_nr = -1;
        std::shared_ptr<KeypointLayer> keypoint;
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, GroupInteract }; }
    };

    struct KeypointLayerGroupRemove : public KeypointLayer {
        LockerIndex removing_layerindx = -1;
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, GroupRemove }; }
    };

    struct KeypointLayerAddSprite : public KeypointLayerAdd {
        inline virtual LayerBuildType layertype() const { return Sprite; }
    };

    struct KeypointLayerSpriteJoin : public KeypointLayer {
        LockerIndex joining_layerindx = -1;
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, SpriteJoin }; }
    };

    struct KeypointLayerSpriteSwap : public KeypointLayer {
        LockerIndex sprite_nr = -1;
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, SpriteSwap }; }
    };

    struct KeypointLayerSpriteSharedInteract : public KeypointLayer {
        std::shared_ptr<KeypointLayer> keypoint;
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, SpriteSharedInteract }; }
    };

    struct KeypointLayerSpriteInteract : public KeypointLayer {
        LockerIndex sprite_nr = -1;
        std::shared_ptr<KeypointLayer> keypoint;
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, SpriteInteract }; }
    };

    struct KeypointLayerSpriteRemove : public KeypointLayer {
        LockerIndex removing_layerindx = -1;
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, SpriteRemove }; }
    };

    struct KeypointLayerAddTexture : public KeypointLayerAdd {
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

    struct KeypointLayerInteractSwap : public KeypointLayer {
        LockerIndex indx = -1;

        enum SwapMode : short {
            KeepNotDeformed,
            SetDefault,
            NewTransform,
            KeepInAspect
        };

        SwapMode swap = KeepInAspect;
        std::unique_ptr<SDL_FRect> swap_rect_ptr = nullptr;
        std::unique_ptr<SDL_FRect> swap_part_ptr = nullptr;

        KeypointLayerInteractSwap() = default;
        KeypointLayerInteractSwap(const KeypointLayerInteractSwap& other)
            : KeypointLayer(other),
            indx(other.indx),
            swap(other.swap),
            swap_rect_ptr(other.swap_rect_ptr ? std::make_unique<SDL_FRect>(*other.swap_rect_ptr) : nullptr),
            swap_part_ptr(other.swap_part_ptr ? std::make_unique<SDL_FRect>(*other.swap_part_ptr) : nullptr) {
        }
        KeypointLayerInteractSwap& operator=(const KeypointLayerInteractSwap& other);
        KeypointLayerInteractSwap(KeypointLayerInteractSwap&&) noexcept = default;
        KeypointLayerInteractSwap& operator=(KeypointLayerInteractSwap&&) noexcept = default;
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, InteractSwap }; }
    };
    
    struct KeypointLayerInteractColor : public KeypointLayer, public KeypointEase {
        SDL_Color color = { 255 };

        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, InteractColor }; }
        inline bool has_ease() override { return true; }
    };

    struct KeypointLayerAddAnimation : public KeypointLayerAdd {
        inline virtual LayerBuildType layertype() const { return Animation; }
    };

    struct KeypointLayerInteractAnimationSpeed : public KeypointLayer, public KeypointEase {
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

    struct KeypointLayerAddText : public KeypointLayerAdd {
        inline virtual LayerBuildType layertype() const { return Text; }
    };

    struct KeypointLayerInteractTextWrap : public KeypointLayer {
        int wrap_limit_px = std::numeric_limits<int>::max();
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, InteractTextWrap }; }
    };

    struct KeypointLayerInteractTextNew : public KeypointLayer {
        const char* new_text;
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, InteractTextNew }; }
    };

    struct KeypointLayerInteractTextAppend : public KeypointLayer {
        const char* text_at_end;
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, InteractTextAppend }; }
    };

    struct KeypointLayerInteractTextInsert : public KeypointLayer {
        size_t offset;
        const char* text_at_insert;
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, InteractTextInsert }; }
    };

    struct KeypointLayerInteractTextSwapFont : public KeypointLayer {
        LockerIndex fontindx;
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Layer, InteractTextSwapFont }; }
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

    inline KeypointLayerInteractSwap& KeypointLayerInteractSwap::operator=(const KeypointLayerInteractSwap& other) {
        if (this == &other) return *this;
        KeypointLayer::operator=(other);
        indx = other.indx;
        swap = other.swap;
        swap_rect_ptr = other.swap_rect_ptr ? std::make_unique<SDL_FRect>(*other.swap_rect_ptr) : nullptr;
        swap_part_ptr = other.swap_part_ptr ? std::make_unique<SDL_FRect>(*other.swap_part_ptr) : nullptr;
        return *this;
    }
}
