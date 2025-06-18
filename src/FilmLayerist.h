#pragma once
#include <vector>
#include <list>
#include <memory>

#include "FilmKeypoint.h"
#include "LockerSimple.h"
#include "FilmScene.h"

// it handles layer stuff, transition between positions, using ease functions, etc...

class FilmLayerist {
    struct KeypointTracker {
        FilmKeypoint* keypoint_ptr;
        FilmKeypoint timer;
    };
public:


    struct Layer {
        SDL_FRect src;
        SDL_FRect rect;

        float(*ease_func_pos)(float);
        float(*ease_func_alpha)(float);
        float(*ease_func_swap)(float);

        ResourceIndex texind;
        uint8_t alpha;

        bool use_from_manager = true;

        struct LayerChange {
            char pos : 1;
            char alpha : 1;
            char swap : 1;
        } change = { 0 };
    };

    inline void setFilmScene(FilmScene* scene) { pFilmScene = scene; }

    void registerLayerKeypoint(FilmKeypoint* keypoint);

    virtual void render() = 0;

private:
    std::vector<Layer> maLayers;
    std::vector<LayerIndex> maActiveLayerIndexes;

    LockerSimple<KeypointTracker> mKeypointPtrLocker;

    FilmScene* pFilmScene;
};

