#include "FilmBackground.h"

void FilmBackground::registerBackgroundKeypoint(FilmKeypointBackground* keypoint) {
    pKeypoint = keypoint;

    switch (pKeypoint->type().specific_type) {
    case FilmKeypointBackground::Swap:
        mTexPrev = mTex;
        mTex = pKeypoint->to;
        mRendMode = pKeypoint->rend_mode;
        transformTexture(mTex, mRendMode);
        break;
    case FilmKeypointBackground::TransparentSwap:
        mEaseTimer.setEase(dynamic_cast<FilmKeypointBgTransparentSwap*>(keypoint)->ease_func);
        mTexPrev = mTex;
        mTex = pKeypoint->to;
        mRendModePrev = mRendMode;
        mRendMode = pKeypoint->rend_mode;
        transformTexture(mTexPrev, mRendModePrev);
        transformTexture(mTex, mRendMode);
        mEaseTimer.start(*dynamic_cast<TimerStep*>(keypoint));
        break;
    }
}

void FilmBackground::update() {
    mEaseTimer.update();

    if (pKeypoint->type().specific_type == FilmKeypointBackground::TransparentSwap) {
        const auto kp = dynamic_cast<FilmKeypointBgTransparentSwap*>(pKeypoint);

        if (kp->from != -1) pTexMgr->GetLockerTexture(kp->from).setAlpha(SDL_clamp((1.f - mEaseTimer) * 255, 0, 255));
        if (kp->to != -1) pTexMgr->GetLockerTexture(kp->to).setAlpha(SDL_clamp(mEaseTimer * 255, 0, 255));
    }
}

void FilmBackground::render() {
    if (pKeypoint->type().specific_type == FilmKeypointBackground::Swap) {
        const auto swapkp = *dynamic_cast<FilmKeypointBgSwap*>(pKeypoint);
        const auto target = swapkp.to;

        if (target != -1) {
            pTexMgr->GetLockerTexture(target).render();
        }
    }

    if (pKeypoint->type().specific_type == FilmKeypointBackground::TransparentSwap) {
        const auto swapkp = *dynamic_cast<FilmKeypointBgTransparentSwap*>(pKeypoint);
        if (swapkp.from != -1) {
            pTexMgr->GetLockerTexture(swapkp.from).render();
        }
        if (swapkp.to != -1) {
            pTexMgr->GetLockerTexture(swapkp.to).render();
        }
    }
}

void FilmBackground::transformTexture(TextureIndex texind, FilmKeypointBackground::RenderMode rend_mode) {
    if (texind == -1) return;

    switch (rend_mode) {
    case FilmKeypointBackground::simple: simplyPutTexture(texind); break;
    case FilmKeypointBackground::centered_black_borders: centerBlackBordersTexture(texind); break;
    }
}

void FilmBackground::simplyPutTexture(TextureIndex texind) {
    auto& tex = pTexMgr->GetLockerTexture(texind);

    tex.setPartialRenderingResolution();
    tex.setOffset(0, 0);
    tex.setWidth(tex.getTexture()->w);
    tex.setHeight(tex.getTexture()->h);
}

void FilmBackground::centerBlackBordersTexture(TextureIndex texind) {
    auto& tex = pTexMgr->GetLockerTexture(texind);
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

