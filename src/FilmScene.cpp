#include "FilmScene.h"

bool FilmScene::create(TextureManager* texmgr, SDL_Rect scr_res, const std::vector<ResourceIndex>& texture_indexes) {
    mpTexMgr = texmgr;
    mTextureIndexes = texture_indexes;
    mScreenResolution = scr_res;
    return true;
}

bool FilmScene::create(TextureManager* texmgr, SDL_Rect scr_res, const std::vector<std::string>& texture_paths) {
    mpTexMgr = texmgr;
    mScreenResolution = scr_res;
    mTextureIndexes.reserve(texture_paths.size());

    ResourceIndex indx;
    for (size_t i = 0; i < texture_paths.size(); i++) {
        indx = mpTexMgr->RequestTextureLoad(texture_paths[i].c_str());
        if (indx == -1) return false;
        mTextureIndexes.push_back(indx);
    }
    return true;
}

void FilmScene::start() {
    mKeypointPtr = 0;
    pKeypoint = (*maKeypoints.begin()).get();
    onNext();
}

void FilmScene::update() {
    if (!isGoing()) return;

    onUpdate();

    //if (target->next_condition.need_input == -1) return;

    //target->next_condition.delay;
}

void FilmScene::next() {
    if (!needNext()) return;
    mKeypointPtr++;
    pKeypoint = maKeypoints[mKeypointPtr].get();

    if(mKeypointPtr < maKeypoints.size()) onNext();
}

void FilmScene::finish() {
    mKeypointPtr = -1;
}

void FilmScene::render() {
    if (!isGoing() || pKeypoint->type() == FilmKeypointType::BlankDelay) return;

    if (pKeypoint->type() == FilmKeypointType::Swap) {
        auto swapkp = *reinterpret_cast<FilmKeypointSwap*>(pKeypoint);
        auto target = swapkp.to;

        if (target != -1) {
            mpTexMgr->GetLockerTexture(mTextureIndexes[swapkp.to]).render();
        }
    }

    if (pKeypoint->type() == FilmKeypointType::TransparentSwap) {
        auto swapkp = *reinterpret_cast<FilmKeypointTransparentSwap*>(pKeypoint);
        if (swapkp.from != -1) {
            mpTexMgr->GetLockerTexture(mTextureIndexes[swapkp.from]).render();
        }
        if (swapkp.to != -1) {
            mpTexMgr->GetLockerTexture(mTextureIndexes[swapkp.to]).render();
        }
    }

#if 0
    SDL_SetTextureBlendMode(mpTexMgr->GetLockerTexture(0).getTexture(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(mpTexMgr->GetRenderer(), 0, 0, 0, 128);
#endif
}

void FilmScene::clear() {
    while(!mTextureIndexes.empty()) {
        mpTexMgr->RequestTextureClean(mTextureIndexes.back());
        mTextureIndexes.pop_back();
    }

    maKeypoints.clear();
}

void FilmScene::centerTexture(ResourceIndex texind) {
    if (texind != -1) {
        auto& tex = mpTexMgr->GetLockerTexture(mTextureIndexes[texind]);
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

void FilmScene::onUpdate() {
    bool finished = false;
    if (mIsFrameDelay) {
        if (!(finished = mFrameDelay <= 0)) mFrameDelay--;
    }
    else { 
        if (!(finished = mDuration.count() < 0)) mDuration -= std::chrono::duration_cast<std::chrono::milliseconds>(mpClock->Now() - mPrev);
    }
    mPrev = mpClock->Now();

    if (finished && (!pKeypoint->need_input)) {
        next();
        return;
    }

    if (pKeypoint->type() == FilmKeypointType::TransparentSwap) {
        auto kp = ((FilmKeypointTransparentSwap*)(pKeypoint));
        float procent;
        if (mIsFrameDelay)
            procent = mFrameDelay / float(kp->frame_delay);
        else
            procent = mDuration.count() / kp->delay.count();

        if(kp->from != -1) mpTexMgr->GetLockerTexture(kp->from).setAlpha(procent * 255);
        if(kp->to != -1) mpTexMgr->GetLockerTexture(kp->to).setAlpha((1.f - procent) * 255);
    }
}

void FilmScene::onNext() {
    mIsFrameDelay = pKeypoint->frame_delay > -1;

    if (mIsFrameDelay) mFrameDelay = pKeypoint->frame_delay;
    else mDuration = pKeypoint->delay;

    if (pKeypoint->type() == FilmKeypointType::Swap) {
        centerTexture(((FilmKeypointSwap*)(pKeypoint))->to);
    }
    if (pKeypoint->type() == FilmKeypointType::TransparentSwap) {
        auto kp = ((FilmKeypointTransparentSwap*)(pKeypoint));
        if (kp->from != -1) {
            mpTexMgr->GetLockerTexture(kp->from).setAlpha(255);
            centerTexture(kp->from);
        }
        if (kp->to != -1) {
            mpTexMgr->GetLockerTexture(kp->to).setAlpha(0);
            centerTexture(kp->to);
        }
    }

    if (mFrameDelay == 0 && !pKeypoint->need_input) next();

}
