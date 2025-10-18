#pragma once
#include <vector>
#include <list>
#include <memory>
#include <deque>

#include "FilmKeypoint.h"
#include "LockerSimple.h"
#include "Clock.h"
#include "easefunc.h"
#include "EaseTracker.h"

// it handles layer stuff, transition between positions, using ease functions, etc...

        
class FilmLayerist : public ClockHolder {
    struct KeypointTracker {
        FilmKeypointLayer* keypoint_ptr;
        // timer would track from layer.ease_tracker directly
        LayerIndex layer_index;
        //LockerIndex index;

        enum TrackerAffectionEnum : char {
            TrRect = (1 << 0),
            TrPart = (1 << 1),
            TrAlpha = (1 << 2),
            TrTexInd = (1 << 3)
        };

        EaseTracker<>* ease_rect_tr_ptr   = nullptr;
        EaseTracker<>* ease_part_tr_ptr   = nullptr;
        EaseTracker<>* ease_alpha_tr_ptr  = nullptr;
        EaseTracker<>* ease_texind_tr_ptr = nullptr;

        inline bool is_finished() const {
            if (ease_rect_tr_ptr && !ease_rect_tr_ptr->isEnded())   return false;
            if (ease_part_tr_ptr && !ease_part_tr_ptr->isEnded())   return false;
            if (ease_alpha_tr_ptr && !ease_alpha_tr_ptr->isEnded())  return false;
            if (ease_texind_tr_ptr && !ease_texind_tr_ptr->isEnded()) return false;
            return true;
        }

        inline void update() {
            if (ease_rect_tr_ptr)   (ease_rect_tr_ptr)   ->update();
            if (ease_part_tr_ptr)   (ease_part_tr_ptr)   ->update();
            if (ease_alpha_tr_ptr)  (ease_alpha_tr_ptr)  ->update();
            if (ease_texind_tr_ptr) (ease_texind_tr_ptr) ->update();
        }
    };

    enum RegLayerKpInterAPosEnum {
        Pos,
        RectPos,
        PartPos
    };
public:
    struct Layer {
        static constexpr float c_ease_no_progress = -0.f;
        static constexpr float c_ease_use_default = 2.f;

        template <typename T>
        struct TransitionableParameter {
            EaseTracker<> ease_tracker;
            T elem_from = { 0 };
            T elem_to = { 0 };

            //inline operator float() const { return ease_progress; }
            //inline void operator=(float progress) { ease_progress = progress; }

            //inline float operator()(float f) { return ease_func ? ease_func(f) : ease_progress; }
            inline void shift_elem() { elem_from = elem_to; }
        };

        TransitionableParameter<SDL_FRect> part;
        TransitionableParameter<SDL_FRect> rect;
        TransitionableParameter<uint8_t> alpha;
        TransitionableParameter<ResourceIndex> texind;

        LockerIndex trackerind = 0;

        inline bool is_default() { 
            return part.ease_tracker.isDefault() 
                && rect.ease_tracker.isDefault()
                && alpha.ease_tracker.isDefault()
                && texind.ease_tracker.isDefault();
        }
        inline bool is_progress() {
            return part.ease_tracker.isProgress()
                || rect.ease_tracker.isProgress()
                || alpha.ease_tracker.isProgress()
                || texind.ease_tracker.isProgress();
        }
        inline bool is_one_progress_ended() {
            return part.ease_tracker.isEnded()
                || rect.ease_tracker.isEnded()
                || alpha.ease_tracker.isEnded()
                || texind.ease_tracker.isEnded();
        }
        inline void set_to_default() {
            part.ease_tracker.setDefault();
            rect.ease_tracker.setDefault();
            alpha.ease_tracker.setDefault();
            texind.ease_tracker.setDefault();
        }
    };

    inline void setTextureManager(TextureManager* texmgr) { pTexMgr = texmgr; }
    void registerLayerKeypoint(FilmKeypointLayer* keypoint);

    inline bool isWaiting() const { return mKeypointPtrLocker.cbegin() == mKeypointPtrLocker.cend(); }
    void update();
    void render();

    FilmTimer getLongestWaiting() const;
private:
    void registerLayerKeypointAdd(FilmKeypointLayerAdd* keypoint);
    void registerLayerKeypointInteractAnyPos(FilmKeypointLayerInteractRect* keypoint, LayerIndex li, RegLayerKpInterAPosEnum enum_pos);
    void registerLayerKeypointInteractAlpha(FilmKeypointLayer* keypoint, LayerIndex li);
    void registerLayerKeypointInteractSwap(FilmKeypointLayer* keypoint, LayerIndex li);

    void finalizeSwap(LockerSimple<FilmLayerist::KeypointTracker>::Iterator iter);
    inline void renderSwapProgression(LayerIndex li, SDL_FRect* res_rect, SDL_FRect* res_part, uint8_t alpha);

    void registerTracker(FilmKeypointLayer* keypoint, LayerIndex li, char tracker_affect_mask);

    SDL_FRect lerpRect(const SDL_FRect& from, const SDL_FRect& to, float t);

private:
    std::vector<Layer> maLayers;
    std::vector<LayerIndex> maActiveLayerIndexes;

    LockerSimple<KeypointTracker> mKeypointPtrLocker;

    TextureManager* pTexMgr;
};

