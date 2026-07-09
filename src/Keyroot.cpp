#include "Keyroot.h"

void Keyroot::start(film::Scene* scene, Loader* loader) {
    assert(pScene != nullptr);
    assert(pLoader != nullptr);
    pScene = scene;
    pLoader = loader;

    if(mIsNewScene) pScene->clear();
    if(mIsNewLoader) {
        pLoader->Clean();
        for(size_t i = 0; i < maNewResLoads.size(); i++) {
            pLoader.PushLoadInQueue();
        }
    }

}
