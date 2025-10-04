#include "FilmScene.h"

bool FilmScene::create(TextureManager* texmgr, ScaleOption scr_res, const std::vector<ResourceIndex>& texture_indexes) {
    pTexMgr = texmgr;
    mTextureIndexes = texture_indexes;
    mScaleOption = scr_res;
    mLayerist.setTextureManager(texmgr);
    mBackground.setTextureManager(texmgr);
    mBackground.setScaleOption(&mScaleOption);

    return true;
}

bool FilmScene::create(TextureManager* texmgr, ScaleOption scr_res, const std::vector<std::string>& texture_paths) {
    pTexMgr = texmgr;
    mScaleOption = scr_res;
    mTextureIndexes.reserve(texture_paths.size());
    
    ResourceIndex indx;
    for (size_t i = 0; i < texture_paths.size(); i++) {
        indx = pTexMgr->RequestTextureLoad(texture_paths[i].c_str());
        if (indx == -1) return false;
        mTextureIndexes.push_back(indx);
    }
    mLayerist.setTextureManager(texmgr);
    mBackground.setTextureManager(texmgr);
    mBackground.setScaleOption(&mScaleOption);

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
    if (!isGoing() || pKeypoint->type().global_type == FilmKeypointChangeType::None) return;

    mBackground.render();
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


void FilmScene::onUpdate() {
    mBackupTimer.decrement_time_frame(mpClock->DeltaTime());

    if (isWaiting() && !mBackupTimer.need_input) {
        next();
        return;
    }

    mBackground.update();
    mLayerist.update();
}

void FilmScene::onNext() {
    auto timer = *((FilmTimer*)pKeypoint);

    if (pKeypoint->type().global_type == FilmKeypointChangeType::Layer) {
        mLayerist.registerLayerKeypoint((FilmKeypointLayer*)pKeypoint);
    }
    if (pKeypoint->type().global_type == FilmKeypointChangeType::Background) {
        mBackground.registerBackgroundKeypoint((FilmKeypointBackground*)pKeypoint);
    }

    if(timer.is_zero() && timer.action == timer.Instant) next();

    switch (timer.action) {
    case timer.First:
    case timer.InInputOrFirst: _FALLTHROUGH
    case timer.InInputAfterFirst: _FALLTHROUGH
    case timer.InInput: _FALLTHROUGH 

        break;
    
    case timer.Await:
    case timer.InInputOrAwait: _FALLTHROUGH
    case timer.InInputAfterAwait: _FALLTHROUGH
        break;
    }

    mBackupTimer.action = timer.action;

    if (mBackupTimer.is_zero() && !mBackupTimer.need_input)
        next();

}
