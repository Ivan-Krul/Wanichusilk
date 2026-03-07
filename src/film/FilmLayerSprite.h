#pragma once
#include "FilmLayerGroup.h"

namespace film {
    class LayerSprite;
}

class film::LayerSprite : public LayerGroup {
public:
    inline void swap(LockerIndex swap_nr = -1) noexcept { mSwapIndex = swap_nr; }
    inline LockerIndex getSwap() const noexcept { return mSwapIndex; }

    bool registerKeypoint(PolyPointerList<LayerBase>& layers, std::vector<PolyPointerList<LayerBase>::Iterator>& ali, LayerIndex li, KeypointLayer* keypoint);

    virtual ~LayerSprite() = default;

protected:
    LockerIndex mSwapIndex = -1;
};



