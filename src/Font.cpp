#include "Font.h"
#include "Logger.h"

bool Font::create(const char* path, float size) {
    if (mpFont) return true;
    mpFallbackFont = nullptr;
    mLanguageSet = nullptr;
    mSpacing = 0;

    mpFont = TTF_OpenFont(path, size);
    if (mpFont == nullptr) {
        Logger log(DEFAULT_LOG_SDL_PATH);
        log.logErrorIn(__FUNCTION__, "Opening new font was failed with %s.", SDL_GetError());
        return true;
    }

    mFlags.direction = AnyDirection;
    mFlags.style_flags = (Style)TTF_GetFontStyle(mpFont);
    mFlags.is_kerning = TTF_GetFontKerning(mpFont);
    mFontSize = size;

    return false;
}

bool Font::create(const Font& font) {
    if (mpFont) return true;
    mpFallbackFont = nullptr;
    mLanguageSet = nullptr;
    mSpacing = 0;

    mpFont = TTF_CopyFont(mpFont);
    if (!mpFont) {
        Logger log(DEFAULT_LOG_SDL_PATH);
        log.logErrorIn(__FUNCTION__, "Cloning font was failed with %s.", SDL_GetError());
        return false;
    }

    mFlags.direction = AnyDirection;
    mFlags.style_flags = (Style)TTF_GetFontStyle(mpFont);
    mFlags.is_kerning = TTF_GetFontKerning(mpFont);
    mFontSize = font.getFontSize();

    return false;
}

bool Font::create(Font&& inst) {
    if (mpFont) return true;
    mpFont = inst.mpFont;
    mpFallbackFont = inst.mpFallbackFont;

    mLanguageSet = inst.mLanguageSet;
    mFontSize = inst.mFontSize;
    mSpacing = inst.mSpacing;

    mFlags = inst.mFlags;

    inst.mpFont = nullptr;
    return false;
}

void Font::setFallbackFont(Font* font) noexcept {
    mpFallbackFont = font;
    if (!TTF_AddFallbackFont(mpFont, mpFallbackFont->mpFont)) logSDLErr(__FUNCTION__);
}

void Font::setLanguageSet(const char* lang_bcp47) noexcept {
    mLanguageSet = lang_bcp47;
    if (!TTF_SetFontLanguage(mpFont, mLanguageSet)) logSDLErr(__FUNCTION__);
}

void Font::setFontSize(float font_px) {
    mFontSize = font_px;
    if(!TTF_SetFontSize(mpFont, mFontSize)) logSDLErr(__FUNCTION__);
}

void Font::setSpacing(short spacing_px) {
    mSpacing = spacing_px;
    TTF_SetFontLineSkip(mpFont, mSpacing);
}

void Font::setStyle(Style style) {
    mFlags.style_flags = style;
    TTF_SetFontStyle(mpFont, mFlags.style_flags);
}

void Font::setDirection(Direction direction) {
    mFlags.direction = direction;
    if (!TTF_SetFontDirection(mpFont, (TTF_Direction)mFlags.direction)) logSDLWrn(__FUNCTION__);
}

void Font::kernFont() {
    mFlags.is_kerning = true;
    TTF_SetFontKerning(mpFont, true);
}

void Font::unkernFont() {
    mFlags.is_kerning = false;
    TTF_SetFontKerning(mpFont, false);
}

void Font::addStyle(Style style) {
    mFlags.style_flags = mFlags.style_flags | style;
    TTF_SetFontStyle(mpFont, mFlags.style_flags);
}

void Font::removeStyle(Style style) {
    mFlags.style_flags = ~((~mFlags.style_flags) | style);
    TTF_SetFontStyle(mpFont, mFlags.style_flags);
}

void Font::clear() {
    if (mpFont)
        TTF_CloseFont(mpFont);

    mpFont = nullptr;
    mpFallbackFont = nullptr;

    mLanguageSet = nullptr;
    mFontSize = -1.f;
    mSpacing = 0;
    mFlags = { Normal, AnyDirection, false };
}

Font::~Font() {
    if (mpFont)
        TTF_CloseFont(mpFont); 
}
