#pragma once
#include "FilmLayerBase.h"

class FilmLayerTexture : public FilmLayerBase {
public:
    //FilmLayerTexture();
    void update() override;
    void render() const override;
    inline void clear() override;

    virtual ~FilmLayerTexture() { clear(); }
private:
    enum PosChangeEnum {
        Pos,
        RectPos,
        PartPos
    };
private:

    void pushPosSetter(FilmKeypointLayerInteractRect* keypoint, PosChangeEnum change);
    void pushTexIndSetter(FilmKeypointLayerInteractSwap* keypoint);

    bool onPushSetter(const FilmKeypointLayer* keypoint) override;
    bool onPushTracker(const LockerIndex ease_indx) override;

    TransitParam<SDL_FRect> mPart;
    TransitParam<SDL_FRect> mRect;
    TransitParam<uint8_t> mAlpha;
    TransitParam<ResourceIndex> mTexInd;

};

