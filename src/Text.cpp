#include "Text.h"
#include "Logger.h"

#include <limits>

Text::Text(Text&& inst) noexcept {
    pEngine = inst.pEngine;
    pFontMgr = inst.pFontMgr;
    mFontIndex = inst.mFontIndex;
    mRect = inst.mRect;
    mpText = inst.mpText;
    mWrapPxLimit = inst.mWrapPxLimit;

    inst.mpText = nullptr;
}

bool Text::create(TTF_TextEngine* engine, FontManager* pmgr, LockerIndex font_indx, const char* text) {
    if (mpText) return true;
    pEngine = engine;
    pFontMgr = pmgr;
    mFontIndex = font_indx;
    mRect = { 0.f };
    mWrapPxLimit = std::numeric_limits<int>::max();

    mpText = TTF_CreateText(pEngine, pFontMgr->GetLockerResource(mFontIndex), text, 0);

    if (!mpText) {
        Logger log(DEFAULT_LOG_SDL_PATH);
        log.logErrorIn(__FUNCTION__, "%s.", SDL_GetError());
        return true;
    }

    int w, h;

    if (!TTF_GetTextSize(mpText, &w, &h)) {
        TTF_DestroyText(mpText);
        Logger log(DEFAULT_LOG_SDL_PATH);
        log.logErrorIn(__FUNCTION__, "%s.", SDL_GetError());
        return true;
    }

    mRect.w = w;
    mRect.h = h;

    return false;
}

void Text::setText(const char* new_text) noexcept {
    if (!TTF_SetTextString(mpText, new_text, 0)) {
        Logger log(DEFAULT_LOG_SDL_PATH);
        log.logErrorIn(__FUNCTION__, "%s.", SDL_GetError());
        return;
    }
}

void Text::setFontMgrIndex(LockerIndex font_ind) noexcept {
    assert(font_ind != -1);
    mFontIndex = font_ind;
    if (!TTF_SetTextFont(mpText, pFontMgr->GetLockerResource(mFontIndex))) {
        Logger log(DEFAULT_LOG_SDL_PATH);
        log.logErrorIn(__FUNCTION__, "%s.", SDL_GetError());
        return;
    }
}

void Text::setWrapPxLimit(int limit) {
    if (!TTF_SetTextWrapWidth(mpText, limit == std::numeric_limits<int>::max() ? 0 : limit)) {
        Logger log(DEFAULT_LOG_SDL_PATH);
        log.logErrorIn(__FUNCTION__, "%s.", SDL_GetError());
        return;
    }
}

void Text::clear() {
    if(mpText) TTF_DestroyText(mpText);
    mpText = nullptr;
    mFontIndex = -1;
}

