#pragma once
#include <vector>
#include <thread>

#include "define.h"
#include "IResourceManager.h"

// bundle that is made to load any resource and give the aspects to the destination

class Loader {
public:
    void PushResourcePathInQueue(IResourceLoadParamConvertor* load, IResourceManager* manager);

    inline const char* GetResourcePath(size_t index) const { return maResMgr.size() > index ? maResMgr.at(index).load.path : ""; }
    inline IResourceManager* GetManager(size_t index) const { return maResMgr.size() > index ? maResMgr.at(index).mgr_ptr : nullptr; }
    LockerIndex GetTranscription(size_t index) const { return maResMgr.size() > index ? maResMgr.at(index).index : -1; }

    IResourceManager* GetRequiredInterface(ResourceManagerAttribute attr) const noexcept;

    void Load();
    void Preprocess();
    void Clean();

    inline size_t Size() const noexcept { return maResMgr.size(); }

    inline bool IsCleaned() const noexcept { return mLoadErrNum == -2; }
    inline bool IsFailed() const noexcept { return mLoadErrNum != -2 && mLoadErrNum != -1; }
    inline bool IsProgress() const noexcept { return mProgress != -1; }
    inline bool IsLoaded() const noexcept { return mLoadErrNum == -1; }
    inline size_t GetFailed() const noexcept { return mLoadErrNum; }
    inline size_t GetProgress() const noexcept { return mProgress; }

private:
    struct ResourceIndexer {
        ResourceLoadParams load;
        IResourceManager* mgr_ptr;
        LockerIndex index;
    };

private:
    void loadMain();

    std::vector<ResourceIndexer> maResMgr;

    std::thread mLoadThread;
    size_t mLoadErrNum = -2;
    size_t mProgress = -1;
};

struct LoaderHolder {
    inline void setLoader(Loader* loader) noexcept { pLoader = loader; }
protected:
    Loader* pLoader;
};
