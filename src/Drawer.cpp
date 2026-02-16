#include "Drawer.h"

size_t Drawer::addColorGroup(SDL_Color color) {
    Group g;
    g.color = color;
    g.type = DrawType::Point;
    maGroups.emplace_back(g);
    return maGroups.size() - 1;
}

void Drawer::addPoint(size_t group, const SDL_FPoint& point) {
    assert(group < maGroups.size());
    maGroups[group].type = DrawType::Point;
    maGroups[group].points.push_back(point);
}

void Drawer::addLine(size_t group, const SDL_FPoint& a, const SDL_FPoint& b) {
    assert(group < maGroups.size());
    maGroups[group].type = DrawType::Line;
    maGroups[group].points.push_back(a);
    maGroups[group].points.push_back(b);

}

void Drawer::addRect(size_t group, const SDL_FPoint& ab, const SDL_FPoint& cd) {
    assert(group < maGroups.size());
    maGroups[group].type = DrawType::Rect;
    if (ab.x > cd.x) {
        maGroups[group].points.push_back(cd);
        maGroups[group].points.push_back(ab - cd);
    }
    else {
        maGroups[group].points.push_back(ab);
        maGroups[group].points.push_back(cd - ab);
    }
}

void Drawer::addRect(size_t group, const SDL_FRect& rect) {
    assert(group < maGroups.size());
    maGroups[group].type = DrawType::Rect;
    maGroups[group].points.push_back(*reinterpret_cast<const SDL_FPoint*>(&rect));
    maGroups[group].points.push_back(*(reinterpret_cast<const SDL_FPoint*>(&rect) + 1));
}

void Drawer::addCircle(size_t group, const SDL_FPoint& c, const SDL_FPoint& r) {
    assert(group < maGroups.size());
    maGroups[group].type = DrawType::Circle;
    auto rad = c - r;
    // abs
    int ri = *reinterpret_cast<int*>(&rad.x);
    ri &= 0x7fffffff;
    rad.x = *reinterpret_cast<float*>(&ri);
    ri = *reinterpret_cast<int*>(&rad.y);
    ri &= 0x7fffffff;
    rad.y = *reinterpret_cast<float*>(&ri);
#ifndef USE_THIRD_PARTY_MATH
    const auto len = sqrtf(rad.x * rad.x + rad.y * rad.y);
    for (size_t i = 0; i < 16; i++) {
        float a = float(i) / 8.f * MATH_PI;
        maGroups[group].points.push_back(SDL_FPoint{ c.x + cosf(a) * len, c.y + sinf(a) * len });
    }
    maGroups[group].points.push_back(maGroups[group].points.at(maGroups[group].points.size() - 16));
#else
#error No math implementation for sqrtf
#endif
}

inline void Drawer::render(size_t group) const {
    assert(group < maGroups.size());
    const auto& groupObj = maGroups.at(group);
    SDL_SetRenderDrawColor(pRenderer, groupObj.color.r, groupObj.color.g, groupObj.color.b, groupObj.color.a);
    switch (groupObj.type) {
    case DrawType::Point:
        SDL_RenderPoints(pRenderer, groupObj.points.data(), groupObj.points.size());
        break;
    case DrawType::Circle: _FALLTHROUGH
    case DrawType::Line:
        SDL_RenderLines(pRenderer, groupObj.points.data(), groupObj.points.size());
        break;
    case DrawType::Rect:
        SDL_RenderRects(pRenderer, reinterpret_cast<const SDL_FRect*>(groupObj.points.data()), groupObj.points.size() >> 1);
        break;
    }
}

void Drawer::render() const {
    for (size_t i = 0; i < maGroups.size(); i++) {
        render(i);
    }
}

void Drawer::removeColorGroup(size_t group) {
    assert(group < maGroups.size());
    maGroups.erase(maGroups.begin() + group);
}
