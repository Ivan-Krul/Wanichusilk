#pragma once
#include <vector>

#include "define.h"
#include "Clock.h"
#include "Loader.h"
#include "film/FilmScene.h"

#include "ImageManager.h"
#include "AnimationManager.h"
#include "FontManager.h"
#include "TextManager.h"

/*

 ~~ Interacter ~~

resource import 
  [vault]{
           [managers][]
                          }

  [keytree] +> {Keypoints} / {Scenes}
            |
            -> {Loader} / {Loader modifications
                             index removal
                             {mgr_ind, res_prm}  
                                                }
                                                 
  [keybranch] -> {idle} {keytree}
              |
              -> {event} {keytree}

*/

struct Vault {
    ImageManager     *p_imgmgr;
    AnimationManager *p_anmmgr;
    FontManager      *p_fntmgr;
    TextManager      *p_txtmgr;
    
    enum ManagerType {
        Image,
        Animation,
        Font,
        Text
    };

    template<typename RP>
    struct ResourceQueue {
        RP params;
        ManagerType type;
    };
};

class Keyroot {
public:
    enum Flag : char {
        None,
        BackFeed,
        Lasting
    };

    using SharedKeypoint  = std::shared_ptr<film::Keypoint>;
    using ResQueueLoad    = Vault::ResourceQueue<ResourceLoadParams>;
    using ResQueueConvert = Vault::ResourceQueue<ResourceConvertParams>;

public:
    inline void create(Vault* using_vault) noexcept { pVault = using_vault; }

    inline void setTimeEnable(TimerStep enable)   noexcept { mTimeEnable = enable; }
    inline void setTimeDisable(TimerStep disable) noexcept { mTimeDisable = disable; }

    inline void setOperationFlag(Flag flag) noexcept { mFlag = flag; }


    inline void setFlagNewScene(bool new_scene = true) noexcept { mIsNewScene = new_scene; }

    template<typename T>
    inline void pushKeypoint(T&& keypoint);

    inline void setKeypoints(const std::vector<SharedKeypoint>& keypoints)  noexcept { maNewKeypoints = keypoints; mIsNewScene = true; }    
    inline void pushKeypoints(const std::vector<SharedKeypoint>& keypoints) noexcept { maNewKeypoints = keypoints; mIsNewScene = false; }
    
    
    inline void setFlagNewLoader(bool new_loader = true) noexcept { mIsNewLoader = new_loader; }

    inline void pullLoaderIndex(size_t index)                          noexcept { maOldLoaderIndexes.push_back(index); }
    inline void pushLoaderLoadEntry(const ResQueueLoad& load)          noexcept { maNewResLoads.emplace_back(load); }
    inline void pushLoaderConvertEntry(const ResQueueConvert& convert) noexcept { maNewResConverts.emplace_back(convert); }

    inline void setPulledLoaderIndexes(const std::vector<size_t>& indexes)                  noexcept { maOldLoaderIndexes = indexes; mIsNewLoader = false; }
    inline void setPushedLoaderLoadEntries(const std::vector<ResQueueLoad>& loads)          noexcept { maNewResLoads = loads; mIsNewLoader = true; }
    inline void setPushedLoaderConvertEntries(const std::vector<ResQueueConvert>& converts) noexcept { maNewResConverts = converts; mIsNewLoader = true; }
    inline void pushLoaderLoadEntries(const std::vector<ResQueueLoad>& loads)               noexcept { maNewResLoads = loads; mIsNewLoader = false; }
    inline void pushLoaderConvertEntries(const std::vector<ResQueueConvert>& converts)      noexcept { maNewResConverts = converts; mIsNewLoader = false; }

    inline TimerStep getTimeEnable()    const noexcept { return mTimeEnable; }
    inline TimerStep getTimeDisable()   const noexcept { return mTimeDisable; }
    inline Flag      getOperationFlag() const noexcept { return mFlag; }

    void start(film::Scene* scene, Loader* loader);
    void update();
    void render();
    void finish();
    

private:
    TimerStep mTimeEnable;
    TimerStep mTimeDisable;
    // requires runtimer for performing conditional entries
    
    std::vector<SharedKeypoint> maNewKeypoints;

    std::vector<size_t>          maOldLoaderIndexes;
    std::vector<ResQueueLoad>    maNewResLoads;
    std::vector<ResQueueConvert> maNewResConverts;

    Vault* pVault = nullptr;
    film::Scene pScene = nullptr;
    Loader* pLoader = nullptr;

    Flag mFlag = None;

    bool mIsNewScene  = false;
    bool mIsNewLoader = false;
};

template<typename T>
inline void Keyroot::pushKeypoint(T&& keypoint) {
    using KeypointType = std::decay_t<T>;
    static_assert(std::is_base_of<film::Keypoint, KeypointType>::value, "you should add a derived struct Keypoint");

    auto ptr = std::make_shared<KeypointType>(std::forward<T>(keypoint));
    maNewKeypoints.emplace_back(std::static_pointer_cast<film::Keypoint>(ptr));
}
