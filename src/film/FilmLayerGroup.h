#pragma once
#include "FilmLayerRect.h"
#include "FilmKeypointLayer.h"
#include "../TextureManager.h"

namespace film {
    class LayerGroup;
}

class film::LayerGroup : public LayerBase {
public:
    LayerGroup(Clock* clock, Layerist* layerist);
    void update() override;
    void render() const override;
    void clear() override;
    inline TimerStep getLongestWaiting() const noexcept override;

    bool join(LockerIndex layerindex);

    virtual ~LayerGroup() { clear(); }

private:
    bool onPushSetter(KeypointLayer* keypoint) override;
    bool onPushTracker(const LockerIndex ease_indx) override;

    std::vector<LockerIndex> mLockerLayers;
};
