#pragma once
#include "FilmKeypoint.h"

namespace film {
    struct KeypointLoaderView : public Keypoint {
        enum Type : short {
            Fill = 1,
        }

        enum KeypointLoaderViewProgressLayout : char {
            None,
            BottomBar,
            Circle
        };

        KeypointLoaderViewProgressLayout progress_layout = None;

        virtual inline KeypointTypeStruct type() const { return { KeypointChangeType::LoaderView, 0 }; }
    };

    struct KeypointLoaderViewFill : public KeypointLoaderView {
        SDL_Color color = { 0, 0, 0, 255 };

        virtual inline KeypointTypeStruct type() const { return { KeypointChangeType::LoaderView, Fill }; }
    };
}