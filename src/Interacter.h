#pragma once
#include <map>

#include "film/FilmScene.h"
#include "define.h"

/*

 ~~ Interacter ~~

  [keytree] +> {Keypoints} / {Scenes}
            |
            -> {Loader} / {Loader modifications}
*/

class Interacter {
public:
    struct Keytree {
        union {
            FilmScene newscene;
            std::vector<std::shared_ptr<Keypoint>> newkeypoints;
        } squence;
        union {
            Loader newloader;
            struct LoaderChange {
                std::vector<size_t> index_removal;
                // addition?
            } loaderchange;
        } resource;
    };

public:


private:


};
