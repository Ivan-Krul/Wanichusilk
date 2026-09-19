#include "Keyroot.h"

void Keyroot::provoke(film::Scene& scene, Loader& loader, Vault& vault) {
    switch(mStemS) {
    case NewScene_NewLoader:
        scene.clear();

        for(int k = 0; k < maNewKeypoints.size(); k++)
            scene.addKeypoint(maNewKeypoints[k]);

        loader.Clean();

        for(int l = 0; l < maNewResLoads.size(); l++)
            loader.PushLoadInQueue(&maNewResLoads[l].params, maNewResLoads[l].manager_ptr);
        for(int c = 0; c < maNewResConverts.size(); c++)
            loader.PushConvertInQueue(&maNewResConverts[c].params, maNewResConverts[c].manager_ptr);
        break;
    case NewScene_SameLoader:
        break;
    case SameScene_NewLoader:
        break;
    case SameScene_SameLoader:
        break;
    }
}
