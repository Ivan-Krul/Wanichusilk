#include "Loader.h"
#include "Loader.h"

void Loader::PushResourcePathInQueue(const char* path, IResourceManager* manager) {
    mLoadErrNum = -2;

    ResourceIndexer ri;
    ri.mgr_ptr = manager;
    ri.path = path;

    maResMgr.push_back(ri);
}

IResourceManager* Loader::GetRequiredInterface(ResourceManagerAttribute attr) const noexcept {
    for (auto& idx : maResMgr) {
        if (static_cast<short>(idx.mgr_ptr->GetAttribute() & attr)) return idx.mgr_ptr;
    }
    return nullptr;
}

void Loader::Load() {
    mLoadErrNum = -2;
    mProgress = 0;
    mLoadThread = std::thread(&Loader::loadMain, this);
    mLoadThread.detach();
}

void Loader::Clean() {
    for (auto i = 0; i < maResMgr.size(); i++) {
        auto& res = maResMgr[i];
        res.mgr_ptr->RequestResourceClean(res.index);
    }
    maResMgr.clear();

    mLoadErrNum = -2;
}

void Loader::loadMain() {
    LockerIndex li_res;
    for (mProgress = 0; mProgress < maResMgr.size(); mProgress++) {
        auto& res = maResMgr[mProgress];
        li_res = res.mgr_ptr->RequestResourceLoad(res.path);

        if (li_res == -1) {
            mLoadErrNum = mProgress;
            mProgress = -1;
            return;
        }

        res.index = li_res;
    }

    mProgress = -1;
    mLoadErrNum = -1;
}
