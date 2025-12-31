#pragma once
#include <thread>
#include <mutex>
#include <future>

#include "define.h"
#include "IResourceManager.h"
#include "PolyPointerList.h"

// bundle that is made to load any resource and give the aspects to the destination

class Loader {
public:
    inline void setOnLoad(void(*onLoad)(void*)) { mfOnLoad = onLoad; }
    inline void setOnFail(void(*onFail)(void*)) { mfOnFail = onFail; }

private:
    PolyPointerList<IResourceManager> maResMgr;

    void (*mfOnLoad)(void*) = nullptr;
    void (*mfOnFail)(void*) = nullptr;
};


