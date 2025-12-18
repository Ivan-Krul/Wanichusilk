#include "FilmScene.h"

bool FilmScene::create(TextureManager* texmgr, ScaleOption scr_res, const std::vector<TextureIndex>& texture_indexes) {
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
    
    TextureIndex indx;
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
    if (!canTriggerNext()) return; // dangerous
    implicitNext();
}

void FilmScene::finish() {
    mKeypointIndex = -1;
}

void FilmScene::render() {
    if (!isGoing()) return;

    mBackground.render();
    mLayerist.render();

#if 0
    SDL_SetTextureBlendMode(pTexMgr->GetLockerTexture(0).getTexture(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(pTexMgr->GetRenderer(), 0, 0, 0, 128);
#endif
}

inline bool FilmScene::canTriggerNext() const {
    if (mBackupTimer.action == TimerStep::InInputOrFirst || mBackupTimer.action == TimerStep::InInputOrAwait) return true;
    if (mBackupTimer.action == TimerStep::InInputAfterFirst || mBackupTimer.action == TimerStep::InInputAfterAwait) {
        if(pKeypoint->type().global_type == FilmKeypointChangeType::None || pKeypoint->action == TimerStep::Exact) return mBackupTimer.is_zero(); // if explicit delay
        else return mBackupTimer.is_zero() || isWaiting();
    }
    return false;
}

void FilmScene::clear() {
    while(!mTextureIndexes.empty()) {
        pTexMgr->RequestTextureClean(mTextureIndexes.back());
        mTextureIndexes.pop_back();
    }

    maKeypoints.clear();
}


inline void FilmScene::implicitNext() {
    mKeypointIndex++;

    if (mKeypointIndex < maKeypoints.size()) {
        pKeypoint = maKeypoints[mKeypointIndex].get();
        onNext();
    }
}

void FilmScene::onUpdate() {
    mBackground.update();
    mLayerist.update();

    mBackupTimer.decrement_time_frame(mpClock->DeltaTime());
    //FilmKP::SDL_Log_FilmTimer(mBackupTimer);

    if (mBackupTimer.action == TimerStep::Exact) {
        if (mBackupTimer.is_zero())
            implicitNext();
        return;
    }

    if ((isWaiting() || mBackupTimer.is_zero()) && !(mBackupTimer.action == TimerStep::InInputAfterFirst || mBackupTimer.action == TimerStep::InInputAfterAwait)) { // DON'T TOUCH IT FOR THE SAKE OF NOT SPENDING ANOTHER 4 DAYS FIXING IT
        implicitNext();
        return;
    }

}

void FilmScene::onNext() {
    const auto timer = *dynamic_cast<TimerStep*>(pKeypoint);

    const TimerStep backg_timer = mBackground.getLongestWaiting();
    const TimerStep layer_timer = mLayerist.getLongestWaiting();

    if (pKeypoint->type().global_type == FilmKeypointChangeType::Layer) {
        mLayerist.registerLayerKeypoint(dynamic_cast<FilmKeypointLayer*>(pKeypoint));
    }
    if (pKeypoint->type().global_type == FilmKeypointChangeType::Background) {
        mBackground.registerBackgroundKeypoint(dynamic_cast<FilmKeypointBackground*>(pKeypoint));
    }

    if (timer.is_zero() && timer.action == timer.Instant) {
        next();
        return;
    }

    switch (timer.action) {
    case timer.First:
    case timer.InInputOrFirst: _FALLTHROUGH
    case timer.InInputAfterFirst: _FALLTHROUGH
        mBackupTimer = ClockFunc::min(backg_timer, layer_timer);
        if (!timer.is_zero()) { // precise copying
            mBackupTimer.frame_delay = std::min<>(mBackupTimer.frame_delay, timer.frame_delay);
            if (timer.need_time_delay) {
                mBackupTimer.delay = std::min<>(mBackupTimer.delay, timer.delay);
            }
        }
        break;
    
    case timer.Await:
    case timer.InInputOrAwait: _FALLTHROUGH
    case timer.InInputAfterAwait: _FALLTHROUGH
        if (timer.is_zero()) {
            mBackupTimer = ClockFunc::max(backg_timer, layer_timer);
        }
        else { // whatever
            mBackupTimer.frame_delay = std::max<>(std::max<>(backg_timer.frame_delay, layer_timer.frame_delay), timer.frame_delay);
            mBackupTimer.delay = std::max<>(std::max<>(backg_timer.delay, layer_timer.delay), timer.delay);
        }
        break;
    case timer.Exact:
        mBackupTimer.frame_delay = timer.frame_delay;
        mBackupTimer.delay = timer.delay;
        break;
    }
    if (mBackupTimer.delay != mBackupTimer.delay.zero()) mBackupTimer.need_time_delay = true;

    mBackupTimer.action = timer.action;
    ClockFunc::SDL_Log_FilmTimer(mBackupTimer);
}
