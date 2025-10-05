#pragma once
#include "TextureManager.h"
#include "FilmKeypoint.h"
#include "Clock.h"
#include "FilmLayerist.h"
#include "FilmBackground.h"
#include "FrameScaling.h"

#include <vector>
#include <string>
#include <memory>

class FilmScene {
public:
    bool create(TextureManager* texmgr, ScaleOption scr_res, const std::vector<ResourceIndex>& texture_indexes);
    bool create(TextureManager* texmgr, ScaleOption scr_res, const std::vector<std::string>& texture_paths);
    void setClock(Clock* clock) { mpClock = clock; mLayerist.setClock(clock); mBackground.setClock(clock); }

    template<typename T>
    void addKeypoint(T&& keypoint);
    template<typename T>
    T getKeypoint(size_t index) const;

    inline size_t keypointCount() const { return maKeypoints.size(); }
    inline size_t textureCount() const { return mTextureIndexes.size(); }

    inline ResourceIndex getTextureIndex(size_t index) const { return mTextureIndexes[index]; }
    inline TextureManager* getTextureManager() const { return pTexMgr; }

    void start();
    void update();
    void next();
    void finish();

    void render();

    inline bool isWaiting() const { return mBackupTimer.is_zero(); }
    inline bool canTriggerNext() const;
    inline bool isGoing() const { return mKeypointIndex != -1 && (mKeypointIndex + 1) < maKeypoints.size(); }
    inline bool isEnded() const { return (mKeypointIndex + 1) >= maKeypoints.size(); }

    void clear();
    ~FilmScene() { clear(); }
private:
    void onUpdate();
    void onNext();

    std::vector<ResourceIndex> mTextureIndexes;
    std::vector<std::shared_ptr<FilmKeypoint>> maKeypoints;
    FilmKeypoint* pKeypoint;

    TextureManager* pTexMgr;

    FilmLayerist mLayerist;
    FilmBackground mBackground;

    ScaleOption mScaleOption;

    size_t mKeypointIndex = -1;
    Clock* mpClock;

    FilmTimer mBackupTimer;
};

template<typename T>
inline void FilmScene::addKeypoint(T&& keypoint) {
    using KeypointType = std::decay_t<T>;
    static_assert(std::is_base_of<FilmKeypoint, KeypointType>::value, "you should add a derived struct FilmKeypoint");

    auto ptr = std::make_shared<KeypointType>(std::forward<T>(keypoint));
    maKeypoints.push_back(std::static_pointer_cast<FilmKeypoint>(ptr));
}

template<typename T>
inline T FilmScene::getKeypoint(size_t index) const {
    static_assert(std::is_base_of<FilmKeypoint, T>::value, "you should add a derived struct FilmKeypoint");

    return *(reinterpret_cast<T*>(maKeypoints[index]));
}
