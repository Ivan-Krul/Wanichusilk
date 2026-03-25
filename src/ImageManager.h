#pragma once
#include "IResourceManager.h"
#include "Image.h"

using ImageIndex = LockerIndex;

// so instead of refitting texture every time for certain operation, you should clone it instead

class ImageManager : public IResourceManager, public IResourceAccesser<Image>, public IRendererGiver, public IResourcePreprocesser {
public:
    struct LoadParamConvertor : public IResourceLoadParamConvertor {
        const char* path;
        
        inline ResourceLoadParams to_param() const noexcept override { return ResourceLoadParams{ path, 0 }; }
    };

public:
    inline void SetRenderer(SDL_Renderer* renderer) noexcept override { mpRenderer = renderer; }
    inline SDL_Renderer* GetRenderer() const noexcept override { return mpRenderer; }
    
    ImageIndex RequestResourceCreate() override { return -1; };
    inline Image* GetLockerResource(ImageIndex index) override { assert(index != -1);  return &mImageLocker[index]; }
    ImageIndex RequestResourceLoad(ResourceLoadParams load) override {
        Image img;
		bool ret = img.createLoad(load.path);
		if (ret) return -1;
		return mImageLocker.pushInLocker(std::move(img));
    }
    
	bool RequestResourcePreprocess(ImageIndex index) override {
        assert(index != -1);
        return mImageLocker[index].preprocess(mpRenderer);
    }
	
    void RequestResourceClean(ImageIndex index) override {
        mImageLocker.popFromLocker(index);
    }

    inline Attribute GetAttribute() const noexcept override { return Attribute::RendererGiver | Attribute::Accesser | Attribute::Preprocesser; }
    
private:
    SDL_Renderer* mpRenderer = nullptr;
    Locker<Image> mImageLocker;
};
