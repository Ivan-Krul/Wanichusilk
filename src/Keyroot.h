#pragma once
#include <vector>

#include "define.h"
#include "Clock.h"
#include "Loader.h"
#include "film/FilmScene.h"

#include "ResourceManagerVault.h"

// handles everything what would happen inside the branching logic
// can add keypoints and add or remove resources
// then it would play back scene using render calls from above
// it has to provoke
class Keyroot {
public:
    enum Flag : char {
        None,
        BackFeed,
        Lasting
    };

    enum StemStatus : char {
        NewScene_NewLoader,
        NewScene_SameLoader,
        SameScene_NewLoader,
        SameScene_SameLoader
    };

    using SharedKeypoint  = std::shared_ptr<film::Keypoint>;
    using Vault = ResourceManagerVault;
    using ResQueueLoad    = Vault::ResourceQueue<ResourceLoadParams>;
    using ResQueueConvert = Vault::ResourceQueue<ResourceConvertParams>;

public:
    inline void setTimeEnable(TimerStep enable)   noexcept { mTimeEnable = enable; }
    inline void setTimeDisable(TimerStep disable) noexcept { mTimeDisable = disable; }

    inline void setOperationFlag(Flag flag) noexcept { mFlag = flag; }
    inline void setStemStatus(StemStatus stem) noexcept { mStemS = stem; }

    template<typename T>
    inline void pushKeypoint(T&& keypoint);
    inline void setKeypoints(const std::vector<SharedKeypoint>& keypoints) noexcept { maNewKeypoints = keypoints; }

    inline void resetLoader() noexcept { maOldLoaderIndexes.clear(); }
    inline void removeLoaderIndex(size_t index) noexcept { maOldLoaderIndexes.push_back(index); }
    inline void addLoaderLoadEntry(const ResQueueLoad& load) noexcept { maNewResLoads.emplace_back(load); }
    inline void addLoaderConvertEntry(const ResQueueConvert& convert) noexcept { maNewResConverts.emplace_back(convert); }

    inline void setRemovedLoaderIndexes(const std::vector<size_t>& indexes)                 noexcept { maOldLoaderIndexes = indexes; }
    inline void setPushedLoaderLoadEntries(const std::vector<ResQueueLoad>& loads)          noexcept { maNewResLoads = loads; }
    inline void setPushedLoaderConvertEntries(const std::vector<ResQueueConvert>& converts) noexcept { maNewResConverts = converts; }

    inline TimerStep getTimeEnable()    const noexcept { return mTimeEnable; }
    inline TimerStep getTimeDisable()   const noexcept { return mTimeDisable; }
    inline Flag      getOperationFlag() const noexcept { return mFlag; }

    void provoke(film::Scene& scene, Loader& loader, Vault& vault);

private:
    TimerStep mTimeEnable;
    TimerStep mTimeDisable;
    // requires runtimer for performing conditional entries

    std::vector<SharedKeypoint> maNewKeypoints;

    std::vector<size_t>          maOldLoaderIndexes;
    std::vector<ResQueueLoad>    maNewResLoads;
    std::vector<ResQueueConvert> maNewResConverts;

    Flag mFlag = None;

    StemStatus mStemS = NewScene_NewLoader;
};

template<typename T>
inline void Keyroot::pushKeypoint(T&& keypoint) {
    using KeypointType = std::decay_t<T>;
    static_assert(std::is_base_of<film::Keypoint, KeypointType>::value, "you should add a derived struct Keypoint");

    auto ptr = std::make_shared<KeypointType>(std::forward<T>(keypoint));
    maNewKeypoints.emplace_back(std::static_pointer_cast<film::Keypoint>(ptr));
}
