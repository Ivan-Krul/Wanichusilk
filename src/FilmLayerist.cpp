#include "FilmLayerist.h"

void FilmLayerist::registerLayerKeypoint(FilmKeypoint* keypoint) {
    assert(keypoint->type() >= FilmKeypointType::LayerAdd && keypoint->type() <= FilmKeypointType::LayerRemove);

    mKeypointPtrLocker.pushInLocker(KeypointTracker{ keypoint, *keypoint });

    LayerIndex li = (keypoint->type() != FilmKeypointType::LayerAdd) ? ((FilmKeypointLayer*)keypoint)->layerindx;

    switch (keypoint->type()) { // it's mess now, but I must go further
    case FilmKeypointType::LayerAdd:
    {
        auto texind = ((FilmKeypointLayerAdd*)keypoint)->texind;
        Layer l;
        l.texind = texind;
        l.rect = pFilmScene->getTextureManager()->GetLockerTexture(pFilmScene->getTextureIndex(texind)).getRect();
        maLayers.push_back(l);
        break;
    }
    case FilmKeypointType::LayerInteractPos:
    {
        auto kp = (FilmKeypointLayerInteractPos*)keypoint;
        maLayers[li].rect = kp->pos;
        maLayers[li].use_from_manager = false;
        maLayers[li].ease_func_pos = kp->ease_func;
        break;
    }
    case FilmKeypointType::LayerInteractSourcePos:
        maLayers[li].src = ((FilmKeypointLayerInteractSourcePos*)keypoint)->src_pos;
        maLayers[li].use_from_manager = false;
        break;
    case FilmKeypointType::LayerInteractAlpha:
    {
        auto kp = (FilmKeypointLayerInteractAlpha*)keypoint;
        maLayers[li].alpha = kp->alpha;
        maLayers[li].use_from_manager = false;
        maLayers[li].ease_func_alpha = kp->ease_func;
        break;
    }
    case FilmKeypointType::LayerInteractTransparentSwap:
        maLayers[li].ease_func_alpha = ((FilmKeypointLayerInteractTransparentSwap*)keypoint)->ease_func;
    case FilmKeypointType::LayerInteractSwap:
        maLayers[li].texind = ((FilmKeypointLayerInteractSwap*)keypoint)->texindx;
        maLayers[li].rect = pFilmScene->getTextureManager()->GetLockerTexture(pFilmScene->getTextureIndex(maLayers[li].texind)).getRect();
        maLayers[li].use_from_manager = true;
        break;
    case FilmKeypointType::LayerEnable:
        maActiveLayerIndexes.push_back(li);
        break;
    case FilmKeypointType::LayerDisable:
        maActiveLayerIndexes.erase(std::find(maActiveLayerIndexes.begin(), maActiveLayerIndexes.end(), li));
        break;
    case FilmKeypointType::LayerInteractDefault:
        maLayers[li].use_from_manager = true;
        break;
    case FilmKeypointType::LayerAwait: break;
    case FilmKeypointType::LayerRemove:
    {
        maLayers.erase(maLayers.begin() + li);
        auto iter = std::find(maActiveLayerIndexes.begin(), maActiveLayerIndexes.end(), li);
        maActiveLayerIndexes.erase(iter);
        mKeypointPtrLocker.popFromLocker(li);
    }
    default: break;
    }

}

