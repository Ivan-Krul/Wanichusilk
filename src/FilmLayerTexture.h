#pragma once
#include "FilmLayerBase.h"
#include "rect_math.h"
#include "TextureManager.h"

class FilmLayerTexture : public FilmLayerBase {
public:
    FilmLayerTexture(Clock* clock, TextureManager* texmgr);
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

    void renderSwap(SDL_FRect* res_rect, SDL_FRect* res_part, uint8_t max_alpha);
    void finalizeSwap(LockerSimple<FilmLayerBase::Tracker>::Iterator iter);

    inline bool areAllTransitParamDefault() { return mPart.is_default() && mRect.is_default() && mAlpha.is_default() && mTexInd.is_default(); }

    TextureManager* pTexMgr;

    TransitParam<SDL_FRect> mPart;
    TransitParam<SDL_FRect> mRect;
    TransitParam<uint8_t> mAlpha;
    TransitParam<ResourceIndex> mTexInd;
};

