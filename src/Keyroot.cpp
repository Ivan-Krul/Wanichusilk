#include "Keyroot.h"

void Keyroot::provoke(film::Scene& scene, Loader& loader, Vault& vault) {
    Logger log(DEFAULT_LOG_PATH);
    log.logInfoInto(__FUNCTION__, "Keyroot was provoked");

    switch(mStemS) {
    case NewScene_NewLoader:
        scene.clear();
        loader.Clear();
        break;
    case NewScene_SameLoader:
        scene.clear();
        break;
    case SameScene_NewLoader:
        loader.Clear();
        break;
    case SameScene_SameLoader:
        break;
    }

    for(int k = 0; k < maNewKeypoints.size(); k++)
        scene.addKeypoint(maNewKeypoints[k]);

    for(int l = 0; l < maNewResLoads.size(); l++)
        loader.PushPureLoadInQueue(maNewResLoads[l].params, maNewResLoads[l].manager_ptr);
    for(int c = 0; c < maNewResConverts.size(); c++)
        loader.PushPureConvertInQueue(maNewResConverts[c].params, maNewResConverts[c].manager_ptr, maNewResConverts[c].from_manager_ptr, maNewResConverts[c].from_index);

}
