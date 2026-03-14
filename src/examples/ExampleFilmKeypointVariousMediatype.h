#pragma once
#include "FilmScene.h"

void PushExampleFilmKeypointVariousMediatype(FilmScene& mScene) {
    film::KeypointLayerAddAnimation aa;
    aa.loaderind = 0;
    mScene.addKeypoint(aa);
    aa.loaderind = 1;
    mScene.addKeypoint(aa);

    film::KeypointLayerAddText at;
    at.loaderind = 5;
    mScene.addKeypoint(at);
    at.loaderind = 6;
    mScene.addKeypoint(at);
    at.loaderind = 7;
    mScene.addKeypoint(at);

    film::KeypointLayerInteractPos p;
    p.layerindx = 1;
    p.rect.x = 200;
    p.rect.y = 200;
    mScene.addKeypoint(p);

    p.layerindx = 2;
    p.rect.x = 100;
    p.rect.y = 50;
    mScene.addKeypoint(p);

    p.layerindx = 4;
    p.rect.x = 0;
    p.rect.y = 300;
    mScene.addKeypoint(p);

    film::KeypointLayerInteractColor c;
    c.layerindx = 4;
    c.color = { 255, 255, 0, 255 };
    mScene.addKeypoint(c);

    film::KeypointLayerInteractAlpha ana;
    ana.layerindx = 0;
    ana.alpha = 128;
    mScene.addKeypoint(ana);

    film::KeypointLayerInteractAnimationLoop anl;
    anl.layerindx = 0;
    mScene.addKeypoint(anl);
    anl.layerindx = 1;
    mScene.addKeypoint(anl);

    film::KeypointLayerInteractAnimationStart ans;
    ans.layerindx = 0;
    mScene.addKeypoint(ans);
    ans.layerindx = 1;
    mScene.addKeypoint(ans);

    film::KeypointLayerEnable e;
    e.layerindx = 0;
    mScene.addKeypoint(e);
    e.layerindx = 1;
    mScene.addKeypoint(e);
    e.layerindx = 2;
    mScene.addKeypoint(e);
    e.layerindx = 3;
    mScene.addKeypoint(e);
    e.layerindx = 4;
    mScene.addKeypoint(e);

    film::Keypoint ts;
    ts.action = ts.InInputAfterAwait;
    ts.frame_delay = 100;
    mScene.addKeypoint(ts);

    film::KeypointLayerAddGroup ag;
    mScene.addKeypoint(ag);

    film::KeypointLayerGroupJoin aj;
    aj.layerindx = 5;
    aj.joining_layerindx = 4;
    mScene.addKeypoint(aj);
    aj.joining_layerindx = 3;
    mScene.addKeypoint(aj);
    aj.joining_layerindx = 1;
    mScene.addKeypoint(aj);
    aj.joining_layerindx = 2;
    mScene.addKeypoint(aj);

    p.layerindx = 5;
    p.rect.x = 200;
    p.rect.y = 200;
    p.ease_func = ease_cubic_out;
    p.delay = std::chrono::seconds(2);
    p.need_time_delay = true;

    film::KeypointLayerGroupSharedInteract gsi;
    gsi.layerindx = 5;
    gsi.delay = std::chrono::seconds(2);
    gsi.need_time_delay = true;
    gsi.action = gsi.InInputAfterAwait;
    gsi.keypoint = std::static_pointer_cast<film::KeypointLayer>(std::make_shared<decltype(p)>(p));
    mScene.addKeypoint(gsi);

    aa.loaderind = 8;
    mScene.addKeypoint(aa);

    film::KeypointLayerAddSprite as;
    mScene.addKeypoint(as);

    film::KeypointLayerSpriteJoin sj;
    sj.layerindx = 7;
    sj.joining_layerindx = 0;
    mScene.addKeypoint(sj);
    sj.joining_layerindx = 6;
    mScene.addKeypoint(sj);

    film::KeypointLayerSpriteSwap ss;
    ss.layerindx = 7;
    ss.sprite_nr = 0;
    mScene.addKeypoint(ss);
    ss.sprite_nr = 1;
    mScene.addKeypoint(ss);

    film::KeypointLayerSpriteInteract si;
    si.layerindx = 7;
    si.sprite_nr = 1;
    si.keypoint = std::static_pointer_cast<film::KeypointLayer>(std::make_shared<decltype(anl)>(anl));
    mScene.addKeypoint(si);
    si.frame_delay = 500;
    si.action = si.InInputAfterAwait;
    si.keypoint = std::static_pointer_cast<film::KeypointLayer>(std::make_shared<decltype(ans)>(ans));
    mScene.addKeypoint(si);
}
