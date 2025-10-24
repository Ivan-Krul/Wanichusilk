#pragma once
#include "FilmLayerBase.h"
#include "rect_math.h"

class FilmLayerTexture : public FilmLayerBase {
public:
    FilmLayerTexture(Clock* clock);
    void update() override;
    void render() const override;
    inline void clear() override;
    inline FilmTimer getLongestWaiting() const noexcept override;

    virtual ~FilmLayerTexture() { clear(); }
private:
    enum PosChangeEnum {
        Pos,
        RectPos,
        PartPos
    };
private:

    inline void pushPosSetter(FilmKeypointLayerInteractRect* keypoint, PosChangeEnum change);
    inline void setRectPartFromKeypointSwapSetter(FilmKeypointLayerInteractRect* keypoint);
    inline void pushTexIndSetter(FilmKeypointLayerInteractSwap* keypoint);

    bool onPushSetter(const FilmKeypointLayer* keypoint) override;

    inline void pushPosTracker(const LockerIndex ease_indx, PosChangeEnum change);

    bool onPushTracker(const LockerIndex ease_indx) override;

    void finalizeSwap(LockerSimple<FilmLayerBase::Tracker>::Iterator iter);

    TransitParam<SDL_FRect> mPart;
    TransitParam<SDL_FRect> mRect;
    TransitParam<uint8_t> mAlpha;
    TransitParam<ResourceIndex> mTexInd;
};

