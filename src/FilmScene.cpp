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
    mKeypointPtr++;

    mFrameDelay = 0;
    if(mKeypointPtr < maKeypoints.size()) onNext();
}

void FilmScene::finish() {
    mKeypointPtr = -1;
}

void FilmScene::render() {
    if (!isGoing() || maKeypoints[mKeypointPtr]->type() == FilmKeypointType::BlankDelay) return;

    if (maKeypoints[mKeypointPtr]->type() == FilmKeypointType::Swap) {
        auto swapkp = *reinterpret_cast<FilmKeypointSwap*>(maKeypoints[mKeypointPtr].get());
        auto target = swapkp.to;

        if (target != -1) {
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

void FilmScene::onNext() {
    mFrameDelay = maKeypoints[mKeypointPtr]->frame_delay;

    if (maKeypoints[mKeypointPtr]->type() == FilmKeypointType::Swap) {
        auto swapkp = *reinterpret_cast<FilmKeypointSwap*>(maKeypoints[mKeypointPtr].get());
        auto target = swapkp.to;
        if (target != -1) {
            auto& tex = mpTexMgr->GetLockerTexture(mTextureIndexes[target]);
            auto scale = 0.f;
            if (mScreenResolution.w > mScreenResolution.h) {
                scale = mScreenResolution.h / float(tex.getTexture()->h);
                tex.setResolution(tex.getTexture()->w * scale, mScreenResolution.h);
                tex.setOffset((tex.getRect().w - tex.getTexture()->w) / 2, 0);
            }
            else {
                scale = mScreenResolution.w / float(tex.getTexture()->w);
                tex.setResolution(mScreenResolution.w, tex.getTexture()->h * scale);
                tex.setOffset(0,(tex.getRect().h - tex.getTexture()->h) / 2);
            }

        }
    }
}
