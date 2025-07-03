#pragma once
#include "TextureManager.h"
#include "FilmKeypoint.h"
#include "Clock.h"
#include "FilmLayerist.h"

#include <vector>
#include <string>
#include <memory>

class FilmScene {
public:
    inline FilmScene() : mFrameDelay(0), mIsFrameDelay(true) {}

    bool create(TextureManager* texmgr, SDL_Rect scr_res, const std::vector<ResourceIndex>& texture_indexes);
    bool create(TextureManager* texmgr, SDL_Rect scr_res, const std::vector<std::string>& texture_paths);
    void setClock(Clock* clock) { mpClock = clock; mLayerist.setClock(clock); }

    template<typename T>
    void addKeypoint(const T keypoint);
    template<typename T>
    T getKeypoint(size_t index) const;

    inline size_t keypointCount() const { return maKeypoints.size(); }
    inline size_t textureCount() const { return mTextureIndexes.size(); }

    inline ResourceIndex getTextureIndex(size_t index) const { return mTextureIndexes[index]; }
    inline TextureManager* getTextureManager() const { return mpTexMgr; }

    void start();
    void update();
    void next();
    void finish();

    void render();

    inline bool needNext() const { return isGoing() ? mFrameDelay == 0 : false; }
    inline bool isGoing() const { return mKeypointPtr != -1 && mKeypointPtr < maKeypoints.size(); }

    void clear();
    ~FilmScene() { clear(); }
private:
    void centerTexture(ResourceIndex texind);
    void onUpdate();
    void onNext();

    std::vector<ResourceIndex> mTextureIndexes;
    std::vector<std::shared_ptr<FilmKeypoint>> maKeypoints;
    FilmKeypoint* pKeypoint;

    TextureManager* mpTexMgr;

    FilmLayerist mLayerist;

    SDL_Rect mScreenResolution;

    size_t mKeypointPtr = -1;
    Clock* mpClock;
    Clock::SteadyClock::time_point mPrev = Clock::SteadyClock::now();

    Clock::Duration mDuration = std::chrono::seconds(0);
    int mFrameDelay : 31;
    int mIsFrameDelay : 1;

};

template<typename T>
inline void FilmScene::addKeypoint(const T keypoint) {
    static_assert(std::is_base_of<FilmKeypoint, T>::value, "you should add a derived struct FilmKeypoint");

    auto ptr = std::static_pointer_cast<FilmKeypoint>(std::make_shared<T>(keypoint));
    maKeypoints.push_back(ptr);
}

template<typename T>
inline T FilmScene::getKeypoint(size_t index) const {
    static_assert(std::is_base_of<FilmKeypoint, T>::value, "you should add a derived struct FilmKeypoint");

    return *(reinterpret_cast<T*>(maKeypoints[index]));
}
