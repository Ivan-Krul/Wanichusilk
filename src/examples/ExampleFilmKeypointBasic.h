#pragma once
#include "FilmScene.h"

inline void PushExampleFilmKeypointBasic(FilmScene& scene) {
    FilmKeypointBgTransparentSwap swap;
    swap.action = FilmTimer::InInputAfterAwait;
    swap.rend_mode = swap.simple;
    swap.ease_func = ease_cubic_out;
    swap.frame_delay = 50;
    swap.from = -1;
    swap.to = 0;
    scene.addKeypoint(swap);
    swap.rend_mode = swap.centered_black_borders;
    swap.action = FilmTimer::Await;
    swap.from = 0;
    swap.to = 1;
    scene.addKeypoint(swap);
    swap.from = 1;
    swap.to = 0;
    scene.addKeypoint(swap);
    {
        FilmKeypointLayerAddTexture la;
        la.texind = 2;
        scene.addKeypoint(la);
    }
    {
        FilmKeypointLayerInteractAlpha lia;
        lia.layerindx = 0;
        lia.alpha = 0;
        scene.addKeypoint(lia);
    }
    {
        FilmKeypointLayerEnable le;
        le.layerindx = 0;
        scene.addKeypoint(le);
    }
    {
        FilmKeypointLayerInteractAlpha lia;
        lia.action = FilmTimer::Await;
        lia.layerindx = 0;
        lia.alpha = 255;
        lia.need_time_delay = true;
        lia.delay = std::chrono::seconds(3);
        lia.ease_func = ease_cubic_in_out;
        scene.addKeypoint(lia);
    }
    {
        FilmKeypointLayerInteractPos lip;
        lip.action = FilmTimer::Await;
        lip.layerindx = 0;
        lip.ease_func = ease_cubic_in;
        lip.frame_delay = 50;
        lip.rect.x = 50;
        lip.rect.y = 200;
        scene.addKeypoint(lip);
        lip.ease_func = ease_linear;
        lip.rect.x = 320;
        lip.rect.y = 180;
        scene.addKeypoint(lip);
        lip.ease_func = ease_cubic_out;
        lip.rect.x = 270;
        lip.rect.y = 40;
        scene.addKeypoint(lip);
    }
    {
        FilmKeypointLayerInteractTransparentSwap lits;
        lits.action = FilmTimer::Await;
        lits.layerindx = 0;
        lits.texindx = 3;
        lits.frame_delay = 50;
        lits.ease_func = ease_sine_in_out;
        lits.swap = FilmKeypointLayerInteractSwap::SwapMode::KeepInAspect;
        scene.addKeypoint(lits);
    }

    swap.from = 0;
    swap.to = 1;
    scene.addKeypoint(swap);
    swap.from = -1;
    swap.to = 1;
    scene.addKeypoint(swap);
    {
        FilmKeypointLayerInteractTransparentSwap lits;
        lits.action = FilmTimer::Await;
        lits.layerindx = 0;
        lits.texindx = -1;
        lits.frame_delay = 30;
        lits.ease_func = ease_sine_in_out;
        lits.swap = FilmKeypointLayerInteractSwap::SwapMode::KeepInAspect;
        scene.addKeypoint(lits);
    }
    swap.action = FilmTimer::InInputAfterAwait;
    swap.to = 0;
    scene.addKeypoint(swap);
    swap.to = 1;
    scene.addKeypoint(swap);
    swap.to = -1;
    scene.addKeypoint(swap);
}
