#pragma once
#include "TextureManager.h"
#include "Keypoint.h"

#include <vector>
#include <string>
#include <memory>

class FilmScene {
public:
    bool create(TextureManager* texmgr, const std::vector<ResourceIndex>& texture_indexes);
    bool create(TextureManager* texmgr, const std::vector<std::string>& texture_paths);

    template<typename T>
    void addKeypoint(const T keypoint);
    template<typename T>
    T getKeypoint(size_t index) const;

    inline size_t keypointCount() const { return maKeypoints.size(); }
    inline size_t textureCount() const { return mTextureIndexes.size(); }

    inline ResourceIndex getTextureIndex(size_t index) const { return mTextureIndexes[index]; }

    void start();
    void update();
    void next();
    void finish();

    void render();

    inline bool needNext() const { return isGoing() ? maKeypoints[mKeypointPtr]->need_input && mFrameDelay == 0 : false; }
    inline bool isGoing() const { return mKeypointPtr != -1 && mKeypointPtr < maKeypoints.size(); }

    void clear();
    ~FilmScene() { clear(); }
private:
    void onNext();

    std::vector<ResourceIndex> mTextureIndexes;
    std::vector<std::shared_ptr<FilmKeypoint>> maKeypoints;

    //std::chrono::steady_clock::time_point mFrameStart;
    //std::chrono::steady_clock::time_point mFrameEnd;

    TextureManager* mpTexMgr;

    size_t mKeypointPtr = -1;
    int mFrameDelay = 0;
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

    return *(reinterpret_cast<T*>(maKeypoints[index].get()));
}
