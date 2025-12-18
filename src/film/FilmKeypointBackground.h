#pragma once
#include "FilmKeypoint.h"

namespace film {
    struct KeypointBackground : public Keypoint {
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

    struct KeypointBgSwap : public KeypointBackground {
        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Background, Swap }; }
    };

    struct KeypointBgTransparentSwap : public KeypointBackground, public KeypointEase {
        TextureIndex from;

        inline KeypointTypeStruct type() const override { return { KeypointChangeType::Background, TransparentSwap }; }
        inline bool has_ease() override { return true; }
    };
}
