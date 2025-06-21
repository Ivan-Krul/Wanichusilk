#pragma once
#include <vector>
#include <list>
#include <memory>

#include "FilmKeypoint.h"
#include "LockerSimple.h"
#include "FilmScene.h"
#include "Clock.h"
#include "easefunc.h"

// it handles layer stuff, transition between positions, using ease functions, etc...

class FilmLayerist {
    struct KeypointTracker {
        FilmKeypoint* keypoint_ptr;
        FilmKeypoint timer;
        LayerIndex layer_index;
    };
public:


    struct Layer {
        SDL_FRect src = { 0.f };
        SDL_FRect rect = { 0.f };
        SDL_FRect rect_from = { 0.f };

        float(*ease_func_pos)(float) = nullptr;
        float(*ease_func_alpha)(float) = nullptr;
        float(*ease_func_swap)(float) = nullptr;

        struct LayerEaseProgress {
            float pos = 0.f;
            float alpha = 0.f;
            float swap = 0.f;
        } ease_progress = { 0.f };

        ResourceIndex texind = -1;
        ResourceIndex texind_from = -1;
        uint8_t alpha = 255;
        uint8_t alpha_from = 255;

        bool use_from_manager = true;

        struct LayerChange {
            char pos : 1;
            char alpha : 1;
            char swap : 1;
        } change = { 0 };
    };

    inline void setFilmScene(FilmScene* scene) { pFilmScene = scene; }
    inline void setClock(Clock* clock) { pClock = clock; }

    void registerLayerKeypoint(FilmKeypoint* keypoint);

    void update();

    void render();

private:
    void registerLayerKeypointAdd(FilmKeypoint* keypoint, LockerIndex kpllocal);
    void registerLayerKeypointInteractPos(FilmKeypoint* keypoint, LayerIndex li);
    void registerLayerKeypointInteractAlpha(FilmKeypoint* keypoint, LayerIndex li);
    float updateTimeProcenting(KeypointTracker& tracker);

private:
    std::vector<Layer> maLayers;
    std::vector<LayerIndex> maActiveLayerIndexes;

    LockerSimple<KeypointTracker> mKeypointPtrLocker;

    Clock::SteadyClock::time_point mPrev = Clock::SteadyClock::now();

    FilmScene* pFilmScene;
    Clock* pClock;
};

