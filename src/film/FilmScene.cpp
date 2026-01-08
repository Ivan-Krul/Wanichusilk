#include "FilmScene.h"

bool film::Scene::create(ScaleOption scr_res, Loader* loader) {
    assert(loader);
    mpLoader = loader;
    mScaleOption = scr_res;
    if (!loader->IsLoaded()) {

    }
    mLayerist.setLoader(loader);
    mBackground.setLoader(loader);
    mBackground.setScaleOption(&mScaleOption);

    return false;
}

void film::Scene::start() {
    mKeypointIndex = 0;
    pKeypoint = (*maKeypoints.begin()).get();
    onNext();
}

void film::Scene::update() {
    if (!isGoing()) return;

    onUpdate();

    //if (target->next_condition.need_input == -1) return;

    //target->next_condition.delay;
}

void film::Scene::next() {
    if (!canTriggerNext()) return; // dangerous
    implicitNext();
}

void film::Scene::finish() {
    mKeypointIndex = -1;
}

void film::Scene::render() {
    if (!isGoing()) return;

    mBackground.render();
    mLayerist.render();
}

inline bool film::Scene::canTriggerNext() const {
    if (mBackupTimer.action == TimerStep::InInputOrFirst || mBackupTimer.action == TimerStep::InInputOrAwait) return true;
    if (mBackupTimer.action == TimerStep::InInputAfterFirst || mBackupTimer.action == TimerStep::InInputAfterAwait) {
        if(pKeypoint->type().global_type == KeypointChangeType::None || pKeypoint->action == TimerStep::Exact) return mBackupTimer.is_zero(); // if explicit delay
        else return mBackupTimer.is_zero() || isWaiting();
    }
    return false;
}

void film::Scene::clear() {
    mpLoader->Clean();
    maKeypoints.clear();
}


inline void film::Scene::implicitNext() {
    mKeypointIndex++;

    if (mKeypointIndex < maKeypoints.size()) {
        pKeypoint = maKeypoints[mKeypointIndex].get();
        onNext();
    }
}

void film::Scene::onUpdate() {
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

void film::Scene::onNext() {
    const auto timer = *dynamic_cast<TimerStep*>(pKeypoint);

    const TimerStep backg_timer = mBackground.getLongestWaiting();
    const TimerStep layer_timer = mLayerist.getLongestWaiting();

    if (pKeypoint->type().global_type == KeypointChangeType::Layer) {
        mLayerist.registerLayerKeypoint(dynamic_cast<KeypointLayer*>(pKeypoint));
    }
    if (pKeypoint->type().global_type == KeypointChangeType::Background) {
        mBackground.registerBackgroundKeypoint(dynamic_cast<KeypointBackground*>(pKeypoint));
    }

    if (timer.is_zero() && timer.action == timer.Instant) {
        next();
        return;
    }

    switch (timer.action) {
    case timer.First:
    case timer.InInputOrFirst:
    case timer.InInputAfterFirst:
        mBackupTimer = clockfunc::min(backg_timer, layer_timer);
        if (!timer.is_zero()) { // precise copying
            mBackupTimer.frame_delay = std::min<>(mBackupTimer.frame_delay, timer.frame_delay);
            if (timer.need_time_delay) {
                mBackupTimer.delay = std::min<>(mBackupTimer.delay, timer.delay);
            }
        }
        break;
    
    case timer.Await:
    case timer.InInputOrAwait:
    case timer.InInputAfterAwait:
        if (timer.is_zero()) {
            mBackupTimer = clockfunc::max(backg_timer, layer_timer);
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
}
