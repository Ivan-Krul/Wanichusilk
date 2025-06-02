#pragma once
#include "TextureManager.h"

#include <vector>
#include <string>

class Scene {
public:
    bool create(TextureManager* texmgr, const std::vector<ResourceIndex>& texture_indexes);
    bool create(TextureManager* texmgr, const std::vector<std::string>& texture_paths);

private:
    std::vector<ResourceIndex> mTextureIndexes;

    TextureManager* mpTexMgr;
};
