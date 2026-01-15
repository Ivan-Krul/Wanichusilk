#pragma once
#include <SDL3/SDL.h>

#include "LockerSimple.h"

enum class ResourceManagerAttribute : short {
    Accesser = 1 << 0,
    RendererGiver = 1 << 1,
};

struct ResourceLoadParams {
    const char* path = nullptr;
    size_t extra = 0;
};

struct IResourceLoadParamConvertor {
    virtual ResourceLoadParams to_param() const noexcept = 0;
};

inline const ResourceManagerAttribute operator|(const ResourceManagerAttribute left, const ResourceManagerAttribute right) {
    return static_cast<ResourceManagerAttribute>(static_cast<short>(left) | static_cast<short>(right));
}

inline const ResourceManagerAttribute operator&(const ResourceManagerAttribute left, const ResourceManagerAttribute right) {
    return static_cast<ResourceManagerAttribute>(static_cast<short>(left) & static_cast<short>(right));
}

struct IResourceManager {
    using Attribute = ResourceManagerAttribute;

    virtual LockerIndex RequestResourceCreate() = 0;
    virtual LockerIndex RequestResourceLoad(ResourceLoadParams load) = 0;

    virtual inline void RequestResourceClean(LockerIndex index) = 0;

    virtual inline Attribute GetAttribute() const noexcept = 0;
};

struct IRendererGiver {
    virtual inline void SetRenderer(SDL_Renderer* renderer) noexcept = 0;
    virtual inline SDL_Renderer* GetRenderer() const noexcept = 0;
};

template<typename T>
struct IResourceAccesser {
    virtual inline T* GetLockerResource(LockerIndex index) = 0;
};


