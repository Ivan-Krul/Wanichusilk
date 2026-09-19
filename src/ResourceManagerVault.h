#pragma once
#include "ImageManager.h"
#include "AnimationManager.h"
#include "FontManager.h"
#include "TextManager.h"

// holds all resource manager instances for the sake of feeding this info to Loader
struct ResourceManagerVault {
    ImageManager     *p_imgmgr;
    AnimationManager *p_anmmgr;
    FontManager      *p_fntmgr;
    TextManager      *p_txtmgr;
    
    enum ManagerType {
        Image,
        Animation,
        Font,
        Text
    };

    template<typename RP>
    struct ResourceQueue {
        void* manager_ptr = nullptr;
        RP params;
        ManagerType type;
    };

};

