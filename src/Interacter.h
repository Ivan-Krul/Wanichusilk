#pragma once
#include <map>

#include "film/FilmScene.h"
#include "define.h"

#include "ImageManager.h"
#include "AnimationManager.h"
#include "FontManager.h"
#include "TextManager.h"

/*

 ~~ Interacter ~~

resource import 
  [vault]{
           [managers][]
                          }

  [keytree] +> {Keypoints} / {Scenes}
            |
            -> {Loader} / {Loader modifications
                             index removal
                             {mgr_ind, res_prm}  
                                                }
                                                 
  [keybranch] -> {idle} {keytree}
              |
              -> {event} {keytree}

*/

struct Vault {
    ImageManager     *p_imgmgr;
    AnimationManager *p_anmmgr;
    FontManager      *p_fntmgr;
    TextManager      *p_txtmgr;
};

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

                std::vector<ResourceLoadParams> new_loads;
                std::vector<ResourceConvertParams> new_converts;
            } loaderchange;
        } resource;
        enum Flag : char {
            None,
            BackFeed,
            Lasting
        } flag = None;
        enum SeqResMode : char {
            NewScene_NewLoader,
            NewScene_ChangeLoader,
            NewKeypoints_NewLoader,
            NewKeypoints_ChangeLoader
        } seq_res_mode;
    };

    struct Keyroot {
        TimerStep enable_in;
        TimerStep disable_in;
        TimerStep interval;
        // requires runtimer for performing conditional entries
        Keytree tree;
    };

public:
    Interacter(Vault using_vault);
    
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

};
