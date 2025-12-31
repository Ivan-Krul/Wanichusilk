#pragma once
#include <vector>
#include <memory>

#include "IResourceManager.h"
#include "BigAnimation.h"
#include "SmallAnimation.h"
#include "LockerSimple.h"

using AnimationIndex = LockerIndex;

class AnimationManager : public IResourceManager, public IResourceAccesser<Animation> {
public:
    inline void SetRenderer(SDL_Renderer* renderer) noexcept { mpRenderer = renderer; }
    inline SDL_Renderer* GetRenderer() const noexcept { return mpRenderer; }

    inline Animation* GetLockerResource(LockerIndex index) override { assert(index != -1);  return mAnimationLocker[index].get(); }
    LockerIndex RequestResourceLoad(const char* path) {
        Animation anim;
        bool ret = anim.create(path, mpRenderer);
        if (!ret) return -1;

        AnimationIndex indx;
        if (anim.isBig())
            indx = mAnimationLocker.pushInLocker(std::make_unique<BigAnimation>());
        else
            indx = mAnimationLocker.pushInLocker(std::make_unique<SmallAnimation>());
        mAnimationLocker[indx]->create(std::move(anim));
        return indx;
    }

    inline void RequestResourceClean(LockerIndex index) {
        mAnimationLocker.popFromLocker(index);
    }

private:
    SDL_Renderer* mpRenderer = nullptr;
    LockerSimple<std::unique_ptr<Animation>> mAnimationLocker;
};
