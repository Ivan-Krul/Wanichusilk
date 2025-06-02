#include "Scene.h"

bool Scene::create(TextureManager* texmgr, const std::vector<ResourceIndex>& texture_indexes) {
    mpTexMgr = texmgr;
    mTextureIndexes = texture_indexes;
    return true;
}

bool Scene::create(TextureManager* texmgr, const std::vector<std::string>& texture_paths) {
    mpTexMgr = texmgr;
    mTextureIndexes.reserve(texture_paths.size());

    ResourceIndex indx;
    for (size_t i = 0; i < texture_paths.size(); i++) {
        indx = mpTexMgr->RequestTextureLoad(texture_paths[i].c_str());
        if (indx == -1) return false;
        mTextureIndexes.push_back(indx);
    }
    return true;
}
