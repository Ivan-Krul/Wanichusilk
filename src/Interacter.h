#pragma once
#include <map>

#include "define.h"
#include "Clock.h"
#include "Loader.h"
#include "film/FilmScene.h"
#include "Keyroot.h"

// a.k.a. Router
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
