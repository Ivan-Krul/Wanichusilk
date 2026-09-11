#pragma once
#include <map>

#include "define.h"
#include "Clock.h"
#include "Loader.h"
#include "film/FilmScene.h"
#include "Keyroot.h"

// a.k.a. Router
// it hears events and based on map - gives the branch it deserves
// while waiting, it can render stuff, and after event would still do render, but for that specific branch
// it hands a current clock, scene and loader for consistent calls of render from above
// it manages branches in terms of provoking them via events, and handing down the branch handling somewhere else
class Interacter : public ClockHolder {
public:
    bool create(Vault using_vault);
    void setClock(Clock* clock) { mpClock = clock; }
    
    void addBranch(size_t event_id = -1);
    Keyroot* extractBranch(size_t event_id);
    void removeBranch(size_t event_id);

    void forceEvent(size_t event_id);

    void start();
    void update();
    void finish();

    void render();
    
private:
    Vault mVault;
    std::map<size_t, Keyroot> maKeyRoots;

    Clock* mpClock = nullptr;
    film::Scene* mpScene = nullptr;
    Loader* mpLoader = nullptr;

};
