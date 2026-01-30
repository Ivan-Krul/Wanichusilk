#include "Text.h"
#include "Logger.h"

Text::Text(Text&& inst) noexcept {
    pEngine = inst.pEngine;
    pFontMgr = inst.pFontMgr;
    mFontIndex = inst.mFontIndex;
    mRect = inst.mRect;
    mText = inst.mText;
    mWrapPxLimit = inst.mWrapPxLimit;

    inst.mText = { 0 };
}

bool Text::create(TTF_TextEngine* engine, FontManager* pmgr, LockerIndex font_indx, const char* text) {
    if (mRect.w < 0.f) return true;
    pEngine = engine;
    pFontMgr = pmgr;
    mFontIndex = font_indx;
    mRect = { -1 };
    mWrapPxLimit = std::numeric_limits<int>::max();
    mText.praw = text;

    return false;
}

bool Text::preprocess() {
    const char* text = mText.praw;
    mText.text = TTF_CreateText(pEngine, pFontMgr->GetLockerResource(mFontIndex), text, 0);

    if (!mText.text) {
        Logger log(DEFAULT_LOG_SDL_PATH);
        log.logErrorIn(__FUNCTION__, "%s.", SDL_GetError());
        return true;
    }

    int w, h;

    if (!TTF_GetTextSize(mText.text, &w, &h)) {
        TTF_DestroyText(mText.text);
        Logger log(DEFAULT_LOG_SDL_PATH);
        log.logErrorIn(__FUNCTION__, "%s.", SDL_GetError());
        return true;
    }

    mRect.w = w;
    mRect.h = h;
    return false;
}

void Text::setText(const char* new_text) noexcept {
    if (mRect.w < 0.f) return;
    if (!TTF_SetTextString(mText.text, new_text, 0)) {
        Logger log(DEFAULT_LOG_SDL_PATH);
        log.logErrorIn(__FUNCTION__, "%s.", SDL_GetError());
        return;
    }
}

void Text::setFontMgrIndex(LockerIndex font_ind) noexcept {
    assert(font_ind != -1);
    if (mRect.w < 0.f) return;
    mFontIndex = font_ind;
    if (!TTF_SetTextFont(mText.text, pFontMgr->GetLockerResource(mFontIndex))) {
        Logger log(DEFAULT_LOG_SDL_PATH);
        log.logErrorIn(__FUNCTION__, "%s.", SDL_GetError());
        return;
    }
}

void Text::setWrapPxLimit(int limit) {
    if (mRect.w < 0.f) return;
    if (!TTF_SetTextWrapWidth(mText.text, limit == std::numeric_limits<int>::max() ? 0 : limit)) {
        Logger log(DEFAULT_LOG_SDL_PATH);
        log.logErrorIn(__FUNCTION__, "%s.", SDL_GetError());
        return;
    }
}

bool Text::appendText(const char* new_text) noexcept {
    if (mRect.w < 0.f) return true;
    if (!TTF_AppendTextString(mText.text, new_text, 0)) {
        Logger log(DEFAULT_LOG_SDL_PATH);
        log.logErrorIn(__FUNCTION__, "%s.", SDL_GetError());
        return true;
    }
    return false;
}

bool Text::insertText(size_t offset, const char* new_text) noexcept {
    if (mRect.w < 0.f) return true;
    if (!TTF_InsertTextString(mText.text, offset, new_text, 0)) {
        Logger log(DEFAULT_LOG_SDL_PATH);
        log.logErrorIn(__FUNCTION__, "%s.", SDL_GetError());
        return true;
    }
    return false;
}

void Text::clear() {
    if (mRect.w < 0.f) return;
    if(mText.text) TTF_DestroyText(mText.text);
    mRect = { -1 };
    mText = { nullptr };
    mFontIndex = -1;
}

