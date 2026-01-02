#include "FilmBackground.h"

void film::Background::registerBackgroundKeypoint(KeypointBackground* keypoint) {
    pKeypoint = keypoint;

    switch (pKeypoint->type().specific_type) {
    case KeypointBackground::Fill:
        mFill = dynamic_cast<KeypointBgFill*>(pKeypoint)->color;
        break;
    case KeypointBackground::TextureSwap:
        mTexPrev = mTex;
        mTex = dynamic_cast<KeypointBgTexSwap*>(pKeypoint)->to;
        mRendMode = dynamic_cast<KeypointBgTexSwap*>(pKeypoint)->rend_mode;
        transformTexture(mTex, mRendMode);
        break;
    case KeypointBackground::TransparentTextureSwap:
        mEaseTimer.setEase(dynamic_cast<KeypointBgTransparentTexSwap*>(keypoint)->ease_func);
        mTexPrev = mTex;
        mTex = dynamic_cast<KeypointBgTexSwap*>(pKeypoint)->to;
        mPrevRendMode = mRendMode;
        mRendMode = dynamic_cast<KeypointBgTransparentTexSwap*>(pKeypoint)->rend_mode;
        transformTexture(mTexPrev, mPrevRendMode);
        transformTexture(mTex, mRendMode);
        mEaseTimer.start(*dynamic_cast<TimerStep*>(keypoint));
        break;
    }
}

void film::Background::update() {
    mEaseTimer.update();

    if (pKeypoint->type().specific_type == KeypointBackground::TransparentTextureSwap) {
        const auto kp = dynamic_cast<KeypointBgTransparentTexSwap*>(pKeypoint);

        if (kp->from != -1) pTexMgr->GetLockerResource(kp->from)->setAlpha(SDL_clamp((1.f - mEaseTimer) * 255, 0, 255));
        if (kp->to != -1) pTexMgr->GetLockerResource(kp->to)->setAlpha(SDL_clamp(mEaseTimer * 255, 0, 255));
    }
}

void film::Background::render() {
    switch (pKeypoint->type().specific_type) {
    case KeypointBackground::Fill: {
        auto color = dynamic_cast<KeypointBgFill*>(pKeypoint)->color;
        SDL_SetRenderDrawColor(pRenderer, color.r, color.g, color.b, 255);
        SDL_RenderClear(pRenderer);
    }   break;
    case KeypointBackground::TextureSwap: {
        const auto target = dynamic_cast<KeypointBgTexSwap*>(pKeypoint)->to;
        if (target != -1) pTexMgr->GetLockerResource(target)->render();
    }   break;
    case KeypointBackground::TransparentTextureSwap: {
        const auto swapkp = *dynamic_cast<KeypointBgTransparentTexSwap*>(pKeypoint);
        if (swapkp.from != -1) pTexMgr->GetLockerResource(swapkp.from)->render();
        if (swapkp.to != -1) pTexMgr->GetLockerResource(swapkp.to)->render();
    }   break;
    default:
        break;
    }

}

void film::Background::transformTexture(TextureIndex texind, KeypointBgTexSwap::RenderMode rend_mode) {
    if (texind == -1) return;

    switch (rend_mode) {
    case KeypointBgTexSwap::simple: simplyPutTexture(texind); break;
    case KeypointBgTexSwap::centered_black_borders: centerBlackBordersTexture(texind); break;
    }
}

void film::Background::simplyPutTexture(TextureIndex texind) {
    auto& tex = *pTexMgr->GetLockerResource(texind);

    tex.setPartialRenderingResolution();
    tex.setOffset(0, 0);
    tex.setWidth(tex.getTexture()->w);
    tex.setHeight(tex.getTexture()->h);
}

void film::Background::centerBlackBordersTexture(TextureIndex texind) {
    auto& tex = *pTexMgr->GetLockerResource(texind);
    assert(pScale != nullptr);

    const auto frame = pScale->getFrameSize();
    const float tex_w = float(tex.getTexture()->w);
    const float tex_h = float(tex.getTexture()->h);

    // Compute scale to fit both dimensions
    const float scale_x = frame.width / tex_w;
    const float scale_y = frame.height / tex_h;
    const float scale = std::min(scale_x, scale_y);  // Ensure it fits within both bounds

    const float scaled_w = tex_w * scale;
    const float scaled_h = tex_h * scale;

    // Set resolution and center offsets
    tex.setResolution(scaled_w, scaled_h);
    tex.setOffset((frame.width - scaled_w) / 2.f, (frame.height - scaled_h) / 2.f);
}

