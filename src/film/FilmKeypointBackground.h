#pragma once
#include "FilmKeypoint.h"

namespace film {
    struct KeypointBackground : public Keypoint {
        enum Type : short {
            Fill = 1,
            TextureSwap,
            TransparentTextureSwap
        };
    };

    struct KeypointBgFill : public KeypointBackground {
        SDL_Color color = { 0 };
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Background, Fill }; }
    };

    struct KeypointBgTexSwap : public KeypointBackground {
        enum RenderMode {
            blank,
            simple,
            centered_black_borders,
            //centered_fill,
            //scroll
        };

        RenderMode rend_mode = blank;

        LockerIndex to = -1;
        inline KeypointTypeStruct type() const { return { KeypointChangeType::Background, TextureSwap }; }
    };

    struct KeypointBgTransparentTexSwap : public KeypointBgTexSwap, public KeypointEase {
        LockerIndex from;

        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Background, TransparentTextureSwap }; }
        inline bool has_ease() override { return true; }
    };
}
