#include "FilmScene.h"

bool FilmScene::create(TextureManager* texmgr, SDL_Rect scr_res, const std::vector<ResourceIndex>& texture_indexes) {
    pTexMgr = texmgr;
    mTextureIndexes = texture_indexes;
    mScreenResolution = scr_res;
    mLayerist.setTextureManager(texmgr);
    return true;
}

bool FilmScene::create(TextureManager* texmgr, SDL_Rect scr_res, const std::vector<std::string>& texture_paths) {
    pTexMgr = texmgr;
    mScreenResolution = scr_res;
    mTextureIndexes.reserve(texture_paths.size());
    
    ResourceIndex indx;
    for (size_t i = 0; i < texture_paths.size(); i++) {
        indx = pTexMgr->RequestTextureLoad(texture_paths[i].c_str());
        if (indx == -1) return false;
        mTextureIndexes.push_back(indx);
    }
    mLayerist.setTextureManager(texmgr);
    return true;
}

void FilmScene::start() {
    mKeypointIndex = 0;
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
    if (!needNext()) return; // dangerous
    mKeypointIndex++;
    pKeypoint = maKeypoints[mKeypointIndex].get();

    if(mKeypointIndex < maKeypoints.size()) onNext();
}

void FilmScene::finish() {
    mKeypointIndex = -1;
}

void FilmScene::render() {
    if (!isGoing() || pKeypoint->type() == FilmKeypointType::BlankDelay) return;

    if (pKeypoint->type() == FilmKeypointType::Swap) {
        auto swapkp = *reinterpret_cast<FilmKeypointSwap*>(pKeypoint);
        auto target = swapkp.to;

        if (target != -1) {
            pTexMgr->GetLockerTexture(mTextureIndexes[swapkp.to]).render();
        }
    }

    if (pKeypoint->type() == FilmKeypointType::TransparentSwap) {
        auto swapkp = *reinterpret_cast<FilmKeypointTransparentSwap*>(pKeypoint);
        if (swapkp.from != -1) {
            pTexMgr->GetLockerTexture(mTextureIndexes[swapkp.from]).render();
        }
        if (swapkp.to != -1) {
            pTexMgr->GetLockerTexture(mTextureIndexes[swapkp.to]).render();
        }
    }
    mLayerist.render();

#if 0
    SDL_SetTextureBlendMode(pTexMgr->GetLockerTexture(0).getTexture(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(pTexMgr->GetRenderer(), 0, 0, 0, 128);
#endif
}

void FilmScene::clear() {
    while(!mTextureIndexes.empty()) {
        pTexMgr->RequestTextureClean(mTextureIndexes.back());
        mTextureIndexes.pop_back();
    }

    maKeypoints.clear();
}

void FilmScene::centerTexture(ResourceIndex texind) {
    if (texind != -1) {
        auto& tex = pTexMgr->GetLockerTexture(mTextureIndexes[texind]);
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
    mTimer.decrement_time_frame(std::chrono::duration_cast<std::chrono::milliseconds>(mpClock->Now() - mPrev));
    mPrev = mpClock->Now();

    if (mTimer.is_zero() && !mTimer.need_input) {
        next();
        return;
    }

    if (pKeypoint->type() == FilmKeypointType::TransparentSwap) {
        auto kp = ((FilmKeypointTransparentSwap*)(pKeypoint));
        float procent;
        
        if (mTimer.need_time_delay) procent = mTimer.delay.count() / kp->delay.count();
        else procent = mTimer.frame_delay / float(kp->frame_delay);

        if (procent < 0.f) procent = 0.f;

        if(kp->from != -1) pTexMgr->GetLockerTexture(kp->from).setAlpha(procent * 255);
        if(kp->to != -1) pTexMgr->GetLockerTexture(kp->to).setAlpha((1.f - procent) * 255);
    }

    mLayerist.update();
}

void FilmScene::onNext() {
#ifdef DEBUG
    SDL_Log("FilmScene handles the keyword %s\n", debugKeypointNames[(int)pKeypoint->type()]);
#endif

    mTimer = *((FilmTimer*)pKeypoint);

    if ((int)(pKeypoint->type()) >= cLayerIndexBegin && (int)(pKeypoint->type()) <= cLayerIndexEnd) {
        mLayerist.registerLayerKeypoint(pKeypoint);
        next();
    }
    if (pKeypoint->type() == FilmKeypointType::Swap) {
        centerTexture(((FilmKeypointSwap*)(pKeypoint))->to);
    }
    if (pKeypoint->type() == FilmKeypointType::TransparentSwap) {
        auto kp = ((FilmKeypointTransparentSwap*)(pKeypoint));
        if (kp->from != -1) {
            pTexMgr->GetLockerTexture(kp->from).setAlpha(255);
            centerTexture(kp->from);
        }
        if (kp->to != -1) {
            pTexMgr->GetLockerTexture(kp->to).setAlpha(0);
            centerTexture(kp->to);
        }
    }

    if (mTimer.is_zero() && !mTimer.need_input)
        next();

}
