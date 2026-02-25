#pragma once
#include "FilmLayerRect.h"
#include "FilmKeypointLayer.h"
#include "../TextureManager.h"
#include "../PolyPointerList.h"

#include <vector>

namespace film {
    class LayerGroup;
}

class film::LayerGroup : public LayerBase {
public:
    void update() override {} // it's a void updater, because other stuff would be updated anyway
    void render() const override {}
    void clear() override { mLockerLayers.clear(); }
    inline TimerStep getLongestWaiting() const noexcept override { return TimerStep{}; }

    bool join(PolyPointerList<LayerBase>::Iterator it);
    bool interact(KeypointLayerGroupInteract* keypoint);
    bool interactAll(KeypointLayerGroupSharedInteract* keypoint);
    bool detach(PolyPointerList<LayerBase>::Iterator it);

    virtual ~LayerGroup() = default;

protected:
    // layerist itself resolves keypoints
    inline bool onPushSetter(KeypointLayer* keypoint) override { return false; }
    inline bool onPushTracker(const LockerIndex ease_indx) override { return false; }

    std::vector<PolyPointerList<LayerBase>::Iterator> mLockerLayers;
};
