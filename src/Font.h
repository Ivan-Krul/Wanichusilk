#pragma once
#include <SDL3_ttf/SDL_ttf.h>


class Font {
public:
    Font() = default;
    inline Font(const char* path, float size) { create(path, size); }
    inline Font() { create(path, size); }

    bool create(const char* path, float size);
    bool create(const Font& font);



    ~Font();
private:
    TTF_Font* mpFont = nullptr;
    Font* mpFallbackFont = { nullptr };

    const char* mLanguageSet = nullptr;
    float mFontSize = -1.f;
    short mSpacing = 0;

    struct {
        char style_flags : 4;
        char direction : 3;
        char is_kerning : 1;
    } mFlags = {TTF_STYLE_NORMAL, TTF_DIRECTION_INVALID, false};

};
