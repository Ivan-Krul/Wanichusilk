#pragma once
#include "FilmKeypoint.h"
#include "FilmLayerist.h"
#include "FilmBackground.h"
#include "../TextureManager.h"
#include "../Clock.h"
#include "../FrameScaling.h"

#include <vector>
#include <string>
#include <memory>

namespace film {
    class Scene;
}

class film::Scene {
public:
    bool create(TextureManager* texmgr, ScaleOption scr_res, const std::vector<TextureIndex>& texture_indexes);
    bool create(TextureManager* texmgr, ScaleOption scr_res, const std::vector<std::string>& texture_paths);
    void setClock(Clock* clock) { mpClock = clock; mLayerist.setClock(clock); mBackground.setClock(clock); }

    template<typename T>
    void addKeypoint(T&& keypoint);
    template<typename T>
    T getKeypoint(size_t index) const;

    inline size_t keypointCount() const { return maKeypoints.size(); }
    inline size_t textureCount() const { return mTextureIndexes.size(); }

    inline TextureIndex getTextureIndex(size_t index) const { return mTextureIndexes[index]; }
    inline TextureManager* getTextureManager() const { return pTexMgr; }

    void start();
    void update();
    void next();
    void finish();

    void render();

    inline bool isWaiting() const { return mLayerist.isWaiting() && mBackground.isWaiting(); }
    inline bool canTriggerNext() const;
    inline bool isGoing() const { return mKeypointIndex != -1 && mKeypointIndex < maKeypoints.size(); }
    inline bool isEnded() const { return mKeypointIndex >= maKeypoints.size(); }

    void clear();
    ~Scene() { clear(); }
private:
    inline void implicitNext();
    void onUpdate();
    void onNext();

    std::vector<TextureIndex> mTextureIndexes;
    std::vector<std::shared_ptr<Keypoint>> maKeypoints;
    Keypoint* pKeypoint;

    TextureManager* pTexMgr;

    Layerist mLayerist;
    Background mBackground;

    ScaleOption mScaleOption;

    size_t mKeypointIndex = -1;
    Clock* mpClock;

    TimerStep mBackupTimer;
};

template<typename T>
inline void film::Scene::addKeypoint(T&& keypoint) {
    using KeypointType = std::decay_t<T>;
    static_assert(std::is_base_of<Keypoint, KeypointType>::value, "you should add a derived struct Keypoint");

    auto ptr = std::make_shared<KeypointType>(std::forward<T>(keypoint));
    maKeypoints.push_back(std::static_pointer_cast<Keypoint>(ptr));
}

template<typename T>
inline T film::Scene::getKeypoint(size_t index) const {
    static_assert(std::is_base_of<Keypoint, T>::value, "you should add a derived struct Keypoint");

    return *dynamic_cast<T*>(maKeypoints[index].get());
}

using FilmScene = film::Scene;
