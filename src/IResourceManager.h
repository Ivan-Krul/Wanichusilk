#pragma once
#include "LockerSimple.h"


struct IResourceManager {
    virtual LockerIndex RequestResourceCreate() = 0;
    virtual LockerIndex RequestResourceLoad(const char* path) = 0;

    virtual inline void RequestResourceClean(LockerIndex index) = 0;
};

template<typename T>
struct IResourceAccesser {
    virtual inline T* GetLockerResource(LockerIndex index) = 0;
};


