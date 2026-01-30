#pragma once
#include "IResourceManager.h"
#include "FontManager.h"
#include "Text.h"

class TextManager : public IResourceManager, public IResourceAccesser<Text>, public IRendererGiver, public IResourcePreprocesser {
public:
    struct LoadParamConvertor : public IResourceLoadParamConvertor {
        const char* text;
        LockerIndex font_indx;
        inline ResourceLoadParams to_param() const noexcept override { return ResourceLoadParams{ text, static_cast<size_t>(font_indx) }; }
    };

public:
    inline void SetRenderer(SDL_Renderer* renderer) noexcept override {
        mpRenderer = renderer;
        mpEngine = TTF_CreateRendererTextEngine(renderer);
        if (!mpEngine) {
            Logger log(DEFAULT_LOG_SDL_PATH);
            log.logErrorIn(__FUNCTION__, "%s.", SDL_GetError());
        }
    }
    inline SDL_Renderer* GetRenderer() const noexcept override { return mpRenderer; }

    void SetFontManager(FontManager* mgr) noexcept { pFontMgr = mgr; }

    LockerIndex RequestResourceCreate() override { return -1; };
    inline Text* GetLockerResource(LockerIndex index) override { assert(index != -1); return &(mTextLocker[index]); }
    LockerIndex RequestResourceLoad(ResourceLoadParams load) override {
        Text text;
        if (text.create(mpEngine, pFontMgr, load.extra, load.path)) {
            Logger log(DEFAULT_LOG_SDL_PATH);
            log.logErrorIn(__FUNCTION__, "%s.", SDL_GetError());
            return -1;
        }
        return mTextLocker.pushInLocker(std::move(text));
    }

    bool RequestResourcePreprocess(LockerIndex index) override {
        assert(index != -1);
        return mTextLocker[index].preprocess();
    }

    inline void RequestResourceClean(LockerIndex index) override { mTextLocker.popFromLocker(index); }

    inline Attribute GetAttribute() const noexcept override { return Attribute::Accesser | Attribute::RendererGiver | Attribute::Preprocesser; }

    inline ~TextManager() {
        if (!mTextLocker.isEmpty()) mTextLocker.clear();
        TTF_DestroyRendererTextEngine(mpEngine);
    }

private:
    LockerSimple<Text> mTextLocker;
    SDL_Renderer* mpRenderer = nullptr;
    TTF_TextEngine* mpEngine = nullptr;
    FontManager* pFontMgr = nullptr;
};

