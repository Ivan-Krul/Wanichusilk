#include "Loader.h"

void Loader::PushResourcePathInQueue(const char* path, IResourceManager* manager) {
    mIsLoaded = false;
    mLoadErrNum = -2;

    ResourceIndexer ri;
    ri.mgr_ptr = manager;
    ri.path = path;

    maResMgr.emplace_back(ri);
}

void Loader::Load() {
    mLoadErrNum = -2;
    mLoadThread.swap(std::thread(&Loader::loadMain, this));
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
    for (mProgress = 0; i < maResMgr.size(); i++) {
        auto& res = maResMgr[i];
        li_res = res.mgr_ptr->RequestResourceLoad(res.path);

        if (li_res == -1) {
            mLoadErrNum = i;
            return;
        }
    }

    mLoadErrNum = -1;
}
