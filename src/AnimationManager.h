#pragma once
#include <vector>
#include <memory>

#include "IResourceManager.h"
#include "BigAnimation.h"
#include "SmallAnimation.h"
#include "ImageManager.h"

using AnimationIndex = LockerIndex;

class AnimationManager : public IResourceManager, public IResourceAccesser<Animation>, public IRendererGiver, public IResourceConvertor, public IResourcePreprocesser {
public:
    struct LoadParamConvertor : public IResourceParamConvertor<ResourceLoadParams> {
        const char* path;
        inline ResourceLoadParams to_param() const noexcept override { return ResourceLoadParams{ path, 0 }; }
    };
	
	struct ConvertParamsConvertor : public IResourceParamConvertor<ResourceConvertParams> {
		size_t frame_count;
		float delay_ms;
		inline ResourceConvertParams to_param() const noexcept override { return ResourceConvertParams{frame_count, *reinterpret_cast<const size_t*>(&delay_ms)}; }
	};

public:
    inline void SetClock(Clock* const clock) { mpClock = clock; }
    inline void SetRenderer(SDL_Renderer* renderer) noexcept override { mpRenderer = renderer; }
    inline SDL_Renderer* GetRenderer() const noexcept override { return mpRenderer; }

    LockerIndex RequestResourceCreate() override { return -1; };
    inline Animation* GetLockerResource(LockerIndex index) override { assert(index != -1);  return mAnimationLocker[index].get(); }
    LockerIndex RequestResourceLoad(ResourceLoadParams load) override {
        Animation anim;
        if (anim.create(load.path)) return -1;

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
	LockerIndex RequestResourceConvert(IResourceManager* mgr, LockerIndex resind, ResourceConvertParams convert) override {
		assert(resind != -1);
		ImageManager* imgmgr = nullptr;
		if(!(imgmgr = dynamic_cast<ImageManager*>(mgr))) {
			Logger log(DEFAULT_LOG_PATH);
			log.logErrorIn(__FUNCTION__, "Convertion was failed because of incompatable manager was fed.");
			return -1;
		}
		
		Image* img = imgmgr->GetLockerResource(resind);
		
		LockerIndex indx = mAnimationLocker.pushInLocker(std::make_unique<SmallAnimation>());
		if(dynamic_cast<SmallAnimation*>(mAnimationLocker[indx].get())->createConvert(convert.value, *reinterpret_cast<float*>(&convert.extra), *img)) {
			mAnimationLocker.popFromLocker(indx);
            return -1;
		}
		
		return indx;
	}
    bool RequestResourcePreprocess(LockerIndex index) override {
        assert(index != -1);
        return mAnimationLocker[index]->preprocess(mpRenderer);
    }

    void RequestResourceClean(LockerIndex index) override { mAnimationLocker.popFromLocker(index); }

    Attribute GetAttribute() const noexcept override { return Attribute::RendererGiver | Attribute::Accesser | Attribute::Preprocesser | Attribute::Convertor; }

private:
    SDL_Renderer* mpRenderer = nullptr;
    Clock* mpClock = nullptr;
    Locker<std::unique_ptr<Animation>> mAnimationLocker; // lockersimple because of unreliable move operations, but wait, we move only pointers, so it's okay?
};
