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

    FilmKeypoint fk;
    fk.action = FilmKeypoint::Exact;
    fk.frame_delay = 500;
    scene.addKeypoint(fk);

    fk.action = FilmKeypoint::InInputAfterFirst;
    scene.addKeypoint(fk);

}
