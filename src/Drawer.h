#pragma once
#include <SDL3/SDL.h>
#include <vector>

#include "define.h"

// Drawer is Manager of primitive drawings: points, lines and rects

inline SDL_FPoint operator+(const SDL_FPoint& a, const SDL_FPoint& b) {
    return SDL_FPoint{ a.x + b.x, a.y + b.y };
}

inline SDL_FPoint operator-(const SDL_FPoint& a, const SDL_FPoint& b) {
    return SDL_FPoint{ a.x - b.x, a.y - b.y };
}

class Drawer {
public:
    enum class DrawType : char {
        Point,
        Line,
        Rect,
        Circle
    };

    struct Group {
        std::vector<SDL_FPoint> points;
        SDL_Color color;
        DrawType type;
    };
public:
    inline void setRenderer(SDL_Renderer* renderer) { pRenderer = renderer; }

    size_t addColorGroup(SDL_Color color);

    void addPoint(size_t group, const SDL_FPoint& point);
    void addLine(size_t group, const SDL_FPoint& a,  const SDL_FPoint& b);
    void addRect(size_t group, const SDL_FPoint& ab, const SDL_FPoint& cd);
    void addRect(size_t group, const SDL_FRect& rect);
    void addCircle(size_t group, const SDL_FPoint& c, const SDL_FPoint& r);

    inline void render(size_t group) const;
    void render() const;

    void removeColorGroup(size_t group);

private:
    std::vector<Group> maGroups;
    SDL_Renderer* pRenderer;
};
