#include "FilmBackground.h"

void FilmBackground::registerBackgroundKeypoint(FilmKeypoint* keypoint) {
    pKeypoint = keypoint;
}

void FilmBackground::update() {
    if (pKeypoint->type().specific_type == FilmKeypointBackground::TransparentSwap) {
        auto kp = ((FilmKeypointBgTransparentSwap*)(pKeypoint));
        float procent;

        if (mTimer.need_time_delay) procent = mTimer.delay.count() / kp->delay.count();
        else procent = mTimer.frame_delay / float(kp->frame_delay);

        if (procent < 0.f) procent = 0.f;

        if (kp->from != -1) pTexMgr->GetLockerTexture(kp->from).setAlpha(procent * 255);
        if (kp->to != -1) pTexMgr->GetLockerTexture(kp->to).setAlpha((1.f - procent) * 255);
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

        if (mScreenResolution.w > mScreenResolution.h) {
            scale = mScreenResolution.h / float(tex.getTexture()->h);
            scaled_w = tex.getTexture()->w * scale;
            scaled_h = mScreenResolution.h;
            tex.setResolution(scaled_w, scaled_h);
            tex.setOffset((mScreenResolution.w - scaled_w) / 2.f, 0.f);
        } else {
            scale = mScreenResolution.w / float(tex.getTexture()->w);
            scaled_w = mScreenResolution.w;
            scaled_h = tex.getTexture()->h * scale;
            tex.setResolution(scaled_w, scaled_h);
            tex.setOffset(0.f, (mScreenResolution.h - scaled_h) / 2.f);
        }
    }
}

