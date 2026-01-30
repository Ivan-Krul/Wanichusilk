#pragma once
#include <vector>
#include <memory>

#include "IResourceManager.h"
#include "BigAnimation.h"
#include "SmallAnimation.h"

using AnimationIndex = LockerIndex;

class AnimationManager : public IResourceManager, public IResourceAccesser<Animation>, public IRendererGiver, public IResourcePreprocesser {
public:
    struct LoadParamConvertor : public IResourceLoadParamConvertor {
        const char* path;
        inline ResourceLoadParams to_param() const noexcept override { return ResourceLoadParams{ path, 0 }; }
    };

public:
    inline void SetClock(Clock* const clock) { mpClock = clock; }
    inline void SetRenderer(SDL_Renderer* renderer) noexcept override { mpRenderer = renderer; }
    inline SDL_Renderer* GetRenderer() const noexcept override { return mpRenderer; }

    LockerIndex RequestResourceCreate() override { return -1; };
    inline Animation* GetLockerResource(LockerIndex index) override { assert(index != -1);  return mAnimationLocker[index].get(); }
    LockerIndex RequestResourceLoad(ResourceLoadParams load) override {
        Animation anim;
        if (anim.create(load.path, mpRenderer)) return -1;

        anim.setClock(mpClock);
        char fail = true;

        AnimationIndex indx;
        if (anim.isBig()) {
            indx = mAnimationLocker.pushInLocker(std::make_unique<BigAnimation>(std::move(anim), fail));
            if (fail) {
                mAnimationLocker.popFromLocker(indx);
                return -1;
            }
        }
        else {
            indx = mAnimationLocker.pushInLocker(std::make_unique<SmallAnimation>(std::move(anim), fail));
            if (fail) {
                mAnimationLocker.popFromLocker(indx);
                return -1;
            }
        }
        
        return indx;
    }
    bool RequestResourcePreprocess(LockerIndex index) override {
        assert(index != -1);
        return mAnimationLocker[index]->preprocess();
    }

    inline void RequestResourceClean(LockerIndex index) { mAnimationLocker.popFromLocker(index); }

    inline Attribute GetAttribute() const noexcept override { return Attribute::RendererGiver | Attribute::Accesser | Attribute::Preprocesser; }

private:
    SDL_Renderer* mpRenderer = nullptr;
    Clock* mpClock = nullptr;
    LockerSimple<std::unique_ptr<Animation>> mAnimationLocker;
};
