#pragma once
#include <stdint.h>
#include <chrono>
#include <memory>

#include "../define.h"
#include "../Clock.h"
#include "../TextureManager.h"

namespace film {
    typedef int LayerIndex;

    enum class KeypointChangeType : short {
        None = 0,
        Utility,
        Background,
        Layer
    };

    struct KeypointTypeStruct {
        inline KeypointTypeStruct() = default;
        inline KeypointTypeStruct(KeypointChangeType glob, short spec) : global_type(glob), specific_type(spec) {}

        KeypointChangeType global_type : 3;
        short specific_type : 13;
    };

    struct Keypoint : public TimerStep {
        inline virtual KeypointTypeStruct type() const { return KeypointTypeStruct{ KeypointChangeType::None, 0 }; }
        inline virtual bool has_ease() { return false; }
    };

    struct KeypointEase {
        float (*ease_func)(float) = nullptr;
    };
}
