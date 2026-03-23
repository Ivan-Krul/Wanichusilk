#pragma once
#include "FilmLayerRect.h"
#include "FilmKeypointLayer.h"
#include "../PolyPointerList.h"

#include <vector>

namespace film {
    class LayerGroup;
}

class film::LayerGroup : public LayerBase {
public:
    void update() override {} // it's a void updater, because other stuff would be updated anyway
    void render() const override {} // people would make this as an argument against inheritance
    void clear() override { mLockerLayers.clear(); }
    inline TimerStep getLongestWaiting() const noexcept override { return TimerStep{}; }

    virtual bool registerKeypoint(PolyPointerList<LayerBase>& layers, std::vector<PolyPointerList<LayerBase>::Iterator>& ali, LayerIndex li, KeypointLayer* keypoint);

    bool join(PolyPointerList<LayerBase>::Iterator it);
    bool interact(LockerIndex group_nr, KeypointLayer* keypoint);
    bool interactAll(KeypointLayer* keypoint);
    bool detach(PolyPointerList<LayerBase>::Iterator it);

    virtual ~LayerGroup() = default;

protected:
    // layerist itself resolves keypoints
    inline bool onPushSetter(KeypointLayer* keypoint) override { return false; }
    inline bool onPushTracker(const LockerIndex ease_indx) override { return false; }

    std::vector<PolyPointerList<LayerBase>::Iterator> mLockerLayers;
};
