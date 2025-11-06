#pragma once
#include "FilmScene.h"

void PushExampleFilmKeypoint2LayerSync(FilmScene& scene) {
    // setup
    FilmKeypointBgSwap bgswap;
    bgswap.rend_mode = FilmKeypointBgSwap::centered_black_borders;
    bgswap.to = 1;
    scene.addKeypoint(bgswap);

    FilmKeypointLayerAddTexture ladd;
    ladd.texind = 2;
    scene.addKeypoint(ladd);
    ladd.texind = 3;
    scene.addKeypoint(ladd);

    FilmKeypointLayerInteractRectPos lrect;
    lrect.layerindx = 0;
    lrect.rect.x = 0;
    lrect.rect.y = DEFAULT_SCR_RES_Y - 100;
    lrect.rect.h = 100;
    lrect.rect.w = 100;
    scene.addKeypoint(lrect);

    lrect.layerindx = 1;
    lrect.rect.x = DEFAULT_SCR_RES_X - 100;
    lrect.rect.y = 0;
    lrect.rect.h = 100;
    lrect.rect.w = 100;
    scene.addKeypoint(lrect);

    FilmKeypointLayerEnable lenb;
    lenb.layerindx = 0;
    scene.addKeypoint(lenb);

    lenb.layerindx = 1;
    scene.addKeypoint(lenb);

    FilmKeypointLayerInteractAlpha laph;
    laph.layerindx = 0;
    laph.alpha = 255;
    scene.addKeypoint(laph);

    laph.layerindx = 1;
    scene.addKeypoint(laph);

    // action
    FilmKeypoint fk;
    fk.action = FilmKeypoint::Exact;
    fk.frame_delay = 50;
    scene.addKeypoint(fk);

    fk.action = FilmKeypoint::InInputAfterFirst;
    scene.addKeypoint(fk);

    for (auto i = 0; i < 3; i++) {
        FilmKeypointLayerInteractPos lpos;
        lpos.layerindx = 0;
        lpos.action = FilmKeypoint::Instant;
        lpos.rect.x = DEFAULT_SCR_RES_X / 2 - 100;
        lpos.rect.y = DEFAULT_SCR_RES_Y / 2 - 50;
        lpos.ease_func = ease_quad_in;
        lpos.frame_delay = 50;
        scene.addKeypoint(lpos);

        lpos.layerindx = 1;
        lpos.action = FilmKeypoint::Await;
        lpos.rect.x = DEFAULT_SCR_RES_X / 2;
        lpos.rect.y = DEFAULT_SCR_RES_Y / 2 - 50;
        lpos.ease_func = ease_quad_in;
        scene.addKeypoint(lpos);

        lpos.layerindx = 0;
        lpos.action = FilmKeypoint::Instant;
        lpos.rect.x = 0;
        lpos.rect.y = 0;
        lpos.ease_func = ease_quad_out;
        scene.addKeypoint(lpos);

        lpos.layerindx = 1;
        lpos.action = FilmKeypoint::Await;
        lpos.rect.x = DEFAULT_SCR_RES_X - 100;
        lpos.rect.y = DEFAULT_SCR_RES_Y - 100;
        lpos.ease_func = ease_quad_out;
        scene.addKeypoint(lpos);


        lpos.layerindx = 0;
        lpos.action = FilmKeypoint::Instant;
        lpos.rect.x = DEFAULT_SCR_RES_X / 2 - 100;
        lpos.rect.y = DEFAULT_SCR_RES_Y / 2 - 50;
        lpos.ease_func = ease_quad_in;
        scene.addKeypoint(lpos);

        lpos.layerindx = 1;
        lpos.action = FilmKeypoint::Await;
        lpos.rect.x = DEFAULT_SCR_RES_X / 2;
        lpos.rect.y = DEFAULT_SCR_RES_Y / 2 - 50;
        lpos.ease_func = ease_quad_in;
        scene.addKeypoint(lpos);

        lpos.layerindx = 0;
        lpos.action = FilmKeypoint::Instant;
        lpos.rect.x = 0;
        lpos.rect.y = DEFAULT_SCR_RES_Y - 100;
        lpos.ease_func = ease_quad_out;
        scene.addKeypoint(lpos);

        lpos.layerindx = 1;
        lpos.action = FilmKeypoint::Await;
        lpos.rect.x = DEFAULT_SCR_RES_X - 100;
        lpos.rect.y = 0;
        lpos.ease_func = ease_quad_out;
        scene.addKeypoint(lpos);

        fk.action = FilmKeypoint::InInputAfterFirst;
        scene.addKeypoint(fk);
    }
}
