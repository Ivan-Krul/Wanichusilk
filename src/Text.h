#pragma once
#include <SDL3_ttf/SDL_ttf.h>
#include "FontManager.h"

class Text {
public:
    inline Text() = default;
    Text(Text&& inst) noexcept;
    inline bool create(TTF_TextEngine* engine, FontManager* pmgr, LockerIndex font_indx) noexcept { create(engine, pmgr, font_indx, ""); }
    bool create(TTF_TextEngine* engine, FontManager* pmgr, LockerIndex font_indx, const char* text);

    inline const char* getText() const noexcept { return mpText ? mpText->text : nullptr; }
    inline float getOffsetX() const noexcept { return mRect.x; }
    inline float getOffsetY() const noexcept { return mRect.y; }
    inline float getWidth() const noexcept { return mRect.w; }
    inline float getHeight() const noexcept { return mRect.h; }
    inline int getWrapPxLimit() const noexcept { return mWrapPxLimit; }
    inline LockerIndex getFontIndex() const noexcept { return mFontIndex; }
    inline FontManager* getFontManager() const noexcept { return pFontMgr; }
    inline TTF_Text* getTextInstance() const noexcept { return mpText; }

    inline void render() const { if (!mpText) return; TTF_DrawRendererText(mpText, mRect.x, mRect.y); }

    void setText(const char* new_text) noexcept;
    void setFontMgrIndex(LockerIndex font_ind) noexcept;
    void setWrapPxLimit(int limit = std::numeric_limits<int>::max());
    inline void setOffsetX(float x) noexcept { mRect.x = x; }
    inline void setOffsetY(float y) noexcept { mRect.y = y; }

    void clear();

    inline ~Text() { if(mpText) TTF_DestroyText(mpText); }

private:
    TTF_TextEngine* pEngine = nullptr;
    TTF_Text* mpText = nullptr;
    FontManager* pFontMgr = nullptr;
    LockerIndex mFontIndex = -1;

    int mWrapPxLimit = std::numeric_limits<int>::max();

    SDL_FRect mRect;
    
};

