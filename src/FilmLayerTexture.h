#pragma once
#include "FilmLayerBase.h"
#include "rect_math.h"
#include "TextureManager.h"

class FilmLayerTexture : public FilmLayerBase {
public:
    FilmLayerTexture(Clock* clock, TextureManager* texmgr, LockerIndex texind);
    void update() override;
    void render() const override;
    inline void clear() override;
    inline bool isWaiting() const noexcept override;
    inline FilmTimer getLongestWaiting() const noexcept override;

    virtual ~FilmLayerTexture() { clear(); }
private:
    enum PosChangeEnum {
        Pos,
        RectPos,
        PartPos
    };
private:

    inline void pushPosSetter(const FilmKeypointLayerInteractRect* keypoint, PosChangeEnum change);
    inline void pushTexIndSetter(FilmKeypointLayerInteractSwap* keypoint);

    bool onPushSetter(FilmKeypointLayer* keypoint) override;

    inline void pushPosTracker(const LockerIndex ease_indx, PosChangeEnum change);

    bool onPushTracker(const LockerIndex ease_indx) override;

    void renderSwap(const SDL_FRect* res_rect, const SDL_FRect* res_part, uint8_t max_alpha) const;
    void finalizeSwap(LockerSimple<FilmLayerBase::Tracker>::Iterator iter);

    inline bool areAllTransitParamDefault() const noexcept { return mPart.is_default() && mRect.is_default() && mAlpha.is_default() && mTexInd.is_default(); }

    TextureManager* pTexMgr;

    TransitParam<SDL_FRect> mPart;
    TransitParam<SDL_FRect> mRect;
    TransitParam<uint8_t> mAlpha;
    TransitParam<ResourceIndex> mTexInd;
};

