#include "FilmBackground.h"

void FilmBackground::registerBackgroundKeypoint(FilmKeypointBackground* keypoint) {
    pKeypoint = keypoint;

    switch (pKeypoint->type().specific_type) {
    case FilmKeypointBackground::Swap:
        mTexPrev = mTex;
        mTex = pKeypoint->to;
        break;
    case FilmKeypointBackground::TransparentSwap:
        mEaseTimer.setEase(((FilmKeypointEase*)keypoint)->ease_func);
        mTexPrev = mTex;
        mTex = pKeypoint->to;
        mEaseTimer.start(*keypoint);
        break;
    }
}

void FilmBackground::update() {
    mEaseTimer.update();

    if (pKeypoint->type().specific_type == FilmKeypointBackground::TransparentSwap) {
        auto kp = ((FilmKeypointBgTransparentSwap*)(pKeypoint));

        if (kp->from != -1) pTexMgr->GetLockerTexture(kp->from).setAlpha((1.f - mEaseTimer) * 255);
        if (kp->to != -1) pTexMgr->GetLockerTexture(kp->to).setAlpha(mEaseTimer * 255);
    }
}

void FilmBackground::render() {
    if (pKeypoint->type().specific_type == FilmKeypointBackground::Swap) {
        auto swapkp = *reinterpret_cast<FilmKeypointBgSwap*>(pKeypoint);
        auto target = swapkp.to;

        if (target != -1) {
            pTexMgr->GetLockerTexture(mRendMode).render();
        }
    }

    if (pKeypoint->type().specific_type == FilmKeypointBackground::TransparentSwap) {
        auto swapkp = *reinterpret_cast<FilmKeypointBgTransparentSwap*>(pKeypoint);
        if (swapkp.from != -1) {
            pTexMgr->GetLockerTexture(mRendModePrev).render();
        }
        if (swapkp.to != -1) {
            pTexMgr->GetLockerTexture(mRendMode).render();
        }
    }
}

void FilmBackground::centerBlackBordersTexture(ResourceIndex texind) {
    if (texind != -1) {
        auto& tex = pTexMgr->GetLockerTexture(texind);
        float scale = 0.f;
        float scaled_w = 0.f;
        float scaled_h = 0.f;

        auto frame = pScale->getFrameSize();

        if (frame.width > frame.height) {
            scale = frame.height / float(tex.getTexture()->h);
            scaled_w = tex.getTexture()->w * scale;
            scaled_h = frame.height;
            tex.setResolution(scaled_w, scaled_h);
            tex.setOffset((frame.width - scaled_w) / 2.f, 0.f);
        } else {
            scale = frame.width / float(tex.getTexture()->w);
            scaled_w = frame.width;
            scaled_h = tex.getTexture()->h * scale;
            tex.setResolution(scaled_w, scaled_h);
            tex.setOffset(0.f, (frame.height - scaled_h) / 2.f);
        }
    }
}

