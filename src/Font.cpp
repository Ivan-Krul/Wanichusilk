#include "Font.h"
#include "Logger.h"

bool Font::create(const char* path, float size) {
    if (mpFont) return true;
    mFallbackFont = { nullptr };
    mLanguageSet = nullptr;
    mSpacing = 0;

    mpFont = TTF_OpenFont(path, size);
    if (mpFont == nullptr) {
        Logger log(DEFAULT_LOG_SDL_PATH);
        log.logErrorIn(__FUNCTION__, "Opening new font was failed with %s.", SDL_GetError());
        return true;
    }

    mFlags.direction = TTF_DIRECTION_INVALID;
    mFlags.style_flags = TTF_GetFontStyle(mpFont);
    mFlags.is_kerning = TTF_GetFontKerning(mpFont);
    mFontSize = size;

    return false;
}

bool Font::create(const Font& font) {
    if (mpFont) return true;
    mpFallbackFont = { nullptr };
    mLanguageSet = nullptr;
    mSpacing = 0;

    TTF_Font* font = TTF_CopyFont(mpFont);
    if (!font) {
        Logger log(DEFAULT_LOG_SDL_PATH);
        log.logErrorIn(__FUNCTION__, "Cloning font was failed with %s.", SDL_GetError());
        return false;
    }

    mFlags.direction = TTF_DIRECTION_INVALID;
    mFlags.style_flags = TTF_GetFontStyle(mpFont);
    mFlags.is_kerning = TTF_GetFontKerning(mpFont);
    mFontSize = size;

    return false;
}

Font::~Font() {
    if (mpFont)
        TTF_CloseFont(mpFont); 
}
