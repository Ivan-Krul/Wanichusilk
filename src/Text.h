#pragma once
#include <SDL3_ttf/SDL_ttf.h>

#include "FontManager.h"

#include <limits>

class Text {
public:
    inline Text() = default;
    Text(Text&& inst) noexcept;
    inline bool create(TTF_TextEngine* engine, FontManager* pmgr, LockerIndex font_indx) noexcept { return create(engine, pmgr, font_indx, ""); }
    bool create(TTF_TextEngine* engine, FontManager* pmgr, LockerIndex font_indx, const char* text);

    bool preprocess();

    inline const char* getText() const noexcept { return mRect.w < 0.f ? mText.text->text : nullptr; }
    inline float getOffsetX() const noexcept { return mRect.x; }
    inline float getOffsetY() const noexcept { return mRect.y; }
    inline float getWidth() const noexcept { return mRect.w; }
    inline float getHeight() const noexcept { return mRect.h; }
    inline int getWrapPxLimit() const noexcept { return mWrapPxLimit; }
    inline LockerIndex getFontIndex() const noexcept { return mFontIndex; }
    inline FontManager* getFontManager() const noexcept { return pFontMgr; }
    inline TTF_Text* getTextInstance() const noexcept { return mText.text; }
    inline SDL_Color getColor() const noexcept { return mTColor; }

    inline void render() const { if (mRect.w < 0.f) return; TTF_DrawRendererText(mText.text, mRect.x, mRect.y); }

    void setText(const char* new_text) noexcept;
    void setFontMgrIndex(LockerIndex font_ind) noexcept;
    void setWrapPxLimit(int limit = std::numeric_limits<int>::max());
    inline void setOffsetX(float x) noexcept { mRect.x = x; }
    inline void setOffsetY(float y) noexcept { mRect.y = y; }
    inline void setOffset(float x, float y) noexcept { mRect.x = x; mRect.y = y; }
    inline void setColor(SDL_Color color) noexcept { mTColor = color; }

    bool appendText(const char* new_text) noexcept;
    bool insertText(size_t offset, const char* new_text) noexcept;

    void clear();

    inline ~Text() { if(mRect.w < 0.f && mText.text) TTF_DestroyText(mText.text); }

private:
    TTF_TextEngine* pEngine = nullptr;
    union {
        TTF_Text* text;
        const char* praw;
    } mText = { nullptr };
    FontManager* pFontMgr = nullptr;
    LockerIndex mFontIndex = -1;

    int mWrapPxLimit = std::numeric_limits<int>::max();
    int mOutLine = 0;
    SDL_Color mTColor = { 255 };


    SDL_FRect mRect = { -1 };
};

