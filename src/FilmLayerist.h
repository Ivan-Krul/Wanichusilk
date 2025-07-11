#pragma once
#include <vector>
#include <list>
#include <memory>

#include "FilmKeypoint.h"
#include "LockerSimple.h"
#include "Clock.h"
#include "easefunc.h"

// it handles layer stuff, transition between positions, using ease functions, etc...

        
class FilmLayerist {
    struct KeypointTracker {
        FilmKeypoint* keypoint_ptr;
        FilmTimer timer;
        LayerIndex layer_index;
        LockerIndex index;
    };
public:
    struct Layer {
        static constexpr float c_ease_no_progress = -0.f;
        static constexpr float c_ease_use_default = 2.f;

        template <typename T>
        struct TransitionableParameter {
            float(*ease_func)(float) = nullptr;
            float ease_progress = c_ease_use_default;
            T elem_from;
            T elem_to;

            inline operator float() const { return ease_progress; }
            inline void operator=(float progress) { ease_progress = progress; }

            inline float operator()(float f) { return ease_func ? ease_func(f) : ease_progress; }
            inline void set_ease(float(*ease)(float)) { if (ease) { ease_func = ease; ease_progress = 0.f; } }
            inline bool is_easing() { return !(ease_progress == c_ease_use_default || ease_progress == c_ease_no_progress); }
            inline bool is_default() { return ease_progress == c_ease_use_default; }
            inline float in_ease() { return ease_func ? ease_func(ease_progress) : ease_progress; }
            inline void shift_elem() { elem_from = elem_to; }
        };

        TransitionableParameter<SDL_FRect> part;
        TransitionableParameter<SDL_FRect> rect;
        TransitionableParameter<uint8_t> alpha;
        TransitionableParameter<ResourceIndex> texind;
        
        inline bool is_default() { return part == c_ease_use_default && rect == c_ease_use_default && alpha == c_ease_use_default && texind == c_ease_use_default; }
        inline void set_to_default() { part = c_ease_use_default; rect = c_ease_use_default; alpha = c_ease_use_default; texind = c_ease_use_default; }

        /*
        SDL_FRect part = { 0.f };
        SDL_FRect part_from = { 0.f };
        SDL_FRect rect = { 0.f };
        SDL_FRect rect_from = { 0.f };

        float(*ease_func_rect)(float) = nullptr;
        float(*ease_func_part)(float) = nullptr;
        float(*ease_func_alpha)(float) = nullptr;
        float(*ease_func_swap)(float) = nullptr;

        struct LayerEaseProgress {
            float rect = 0.f;
            float part = 0.f;
            float alpha = 0.f;
            float swap = 0.f;
        } ease_progress = { 0.f };

        ResourceIndex texind = -1;
        ResourceIndex texind_from = -1;
        uint8_t alpha = 255;
        uint8_t alpha_from = 255;

        bool use_from_manager = true;

        struct LayerChange {
            char rect : 1;
            char part : 1;
            char alpha : 1;
            char swap : 1;
        } change = { 0 };

        struct LayerUsage {
            char rect : 1;
            char part : 1;
        } usage = { 0 };
        */
    };

    inline void setTextureManager(TextureManager* texmgr) { pTexMgr = texmgr; }
    inline void setClock(Clock* clock) { pClock = clock; }

    void registerLayerKeypoint(FilmKeypoint* keypoint);


    void update();

    void render();

    FilmTimer getLongestWaiting() const;
private:
    void registerLayerKeypointAdd(FilmKeypointLayerAdd* keypoint);
    void registerLayerKeypointInteractAnyPos(FilmKeypointLayerInteractRect* keypoint, LayerIndex li, bool is_src = false);
    void registerLayerKeypointInteractAlpha(FilmKeypoint* keypoint, LayerIndex li);
    float updateTimeProcenting(KeypointTracker& tracker);

    void registerTracker(FilmKeypoint* keypoint, LayerIndex li);

    SDL_FRect lerpRect(const SDL_FRect& from, const SDL_FRect& to, float t);

private:
    std::vector<Layer> maLayers;
    std::vector<LayerIndex> maActiveLayerIndexes;

    LockerSimple<KeypointTracker> mKeypointPtrLocker;

    Clock::SteadyClock::time_point mPrev = Clock::SteadyClock::now();

    TextureManager* pTexMgr;
    Clock* pClock;
};

