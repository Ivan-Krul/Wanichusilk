#pragma once
#include <utility>

#include <SDL3_ttf/SDL_ttf.h>

class Font {
public:
    enum Style : char {
        Normal = TTF_STYLE_NORMAL,
        Bold = TTF_STYLE_BOLD,
        Italic = TTF_STYLE_ITALIC,
        Underline = TTF_STYLE_UNDERLINE,
        Strikethrough = TTF_STYLE_STRIKETHROUGH
    };

    enum Direction : char {
        AnyDirection = TTF_DIRECTION_INVALID,
        LeftToRight = TTF_DIRECTION_LTR,
        RightToLeft = TTF_DIRECTION_RTL,
        TopToBottom = TTF_DIRECTION_TTB,
        BottomToTop = TTF_DIRECTION_BTT
    };

public:
    Font() = default;
    inline Font(const char* path, float size) { create(path, size); }
    inline Font(const Font& font) { create(font); }
    inline Font(Font&& font) noexcept { create(std::move(font)); }

    bool create(const char* path, float size);
    bool create(const Font& font);
    bool create(Font&& inst);

    void setFallbackFont(Font* font) noexcept;
    void setLanguageSet(const char* lang_bcp47) noexcept;
    void setFontSize(float font_px);
    void setSpacing(short spacing_px);
    void setStyle(Style style);
    void setDirection(Direction direction);
    void kernFont();
    void unkernFont();

    void addStyle(Style style);
    void removeStyle(Style style);

    inline Font* getFallbackFont() const noexcept { return mpFallbackFont; }
    inline const char* getLanguageSet() const noexcept { return mLanguageSet; }
    inline float getFontSize() const noexcept { return mFontSize; }
    inline short getSpacing() const noexcept { return mSpacing; }
    inline Style getStyle() const noexcept { return mFlags.style_flags; }
    inline Direction getDirection() const noexcept { return mFlags.direction; }
    inline TTF_Font* getFont() const noexcept { return mpFont; }

    void clear();

    ~Font();
private:
    TTF_Font* mpFont = nullptr;
    Font* mpFallbackFont = nullptr;

    const char* mLanguageSet = nullptr;
    float mFontSize = -1.f;
    short mSpacing = 0;

    struct {
        Style style_flags : 4;
        Direction direction : 3;
        char is_kerning : 1;
    } mFlags = {Normal, AnyDirection, false};

};

inline const Font::Style operator|(const Font::Style left, const Font::Style right) {
    return static_cast<Font::Style>(static_cast<char>(left) | static_cast<char>(right));
}

inline const Font::Style operator~(const Font::Style value) {
    return static_cast<Font::Style>(~static_cast<char>(value));
}
