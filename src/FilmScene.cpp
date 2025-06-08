#include "FilmScene.h"

bool FilmScene::create(TextureManager* texmgr, const std::vector<ResourceIndex>& texture_indexes) {
    mpTexMgr = texmgr;
    mTextureIndexes = texture_indexes;
    return true;
}

bool FilmScene::create(TextureManager* texmgr, const std::vector<std::string>& texture_paths) {
    mpTexMgr = texmgr;
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
}

void FilmScene::update() {
    if (!isGoing()) return;

    if (mFrameDelay) mFrameDelay--;

    auto target = maKeypoints[mKeypointPtr];

    //if (target->next_condition.need_input == -1) return;

    //target->next_condition.delay;
}

void FilmScene::next() {
    if (!needNext()) return;

    mFrameDelay = 0;
    mKeypointPtr++;
    onNext();
}

void FilmScene::finish() {
    mKeypointPtr = -1;
}

void FilmScene::render() {
    if (!isGoing() || maKeypoints[mKeypointPtr]->type() == FilmKeypointType::BlankDelay) return;

    if (maKeypoints[mKeypointPtr]->type() == FilmKeypointType::Swap) {
        auto swapkp = *reinterpret_cast<FilmKeypointSwap*>(maKeypoints[mKeypointPtr].get());
        auto target = swapkp.to;

        if (target != -1)
            mpTexMgr->GetLockerTexture(mTextureIndexes[swapkp.to]).render();
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

void FilmScene::onNext() {
    mFrameDelay = maKeypoints[mKeypointPtr]->frame_delay;
}
