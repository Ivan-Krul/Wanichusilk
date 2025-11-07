#pragma once
#include <limits>

#include "Clock.h"
#include "LockerSimple.h"
#include "FilmKeypoint.h"
#include "EaseTracker.h"

class FilmLayerBase : public ClockHolder {
public:
    template<typename T>
    inline bool pushSetter(T* keypoint);
    template<typename T>
    inline bool pushTracker(T* keypoint);

    virtual void update() = 0;
    virtual void render() const = 0;
    virtual inline bool isWaiting() const noexcept { return maEases.isEmpty(); }
    virtual inline TimerStep getLongestWaiting() const noexcept { return TimerStep(); }
    virtual inline void clear() { maEases.clear(); }

    virtual ~FilmLayerBase() = default;
protected:
    template <typename T>
    struct TransitParam {
        EaseTracker<> ease_tracker;
        T elem_from = { 0 };
        T elem_to = { 0 };
        size_t unused_padding = std::numeric_limits<size_t>::max();

        inline void shift_elem() { elem_from = elem_to; }
        inline bool is_default()  const { return ease_tracker.isDefault(); }
        inline bool is_progress() const { return ease_tracker.isProgress(); }
        inline bool is_ended()    const { return ease_tracker.isEnded(); }
        inline void set_default() { ease_tracker.setDefault(); }
        inline void reset_tracker() { ease_tracker.reset(); }
        inline void clear() { ease_tracker.setDefault(); elem_from = { 0 }; elem_to = { 0 }; }
    };

    struct Tracker {
        EaseTracker<>* ease = nullptr;
        FilmKeypointLayer* keypoint;
    };

protected:
    virtual bool onPushSetter(FilmKeypointLayer* keypoint) = 0;
    virtual bool onPushTracker(const LockerIndex ease_indx) = 0;

    LockerSimple<Tracker> maEases;
};

template<typename T>
inline bool FilmLayerBase::pushSetter(T* keypoint) {
    static_assert(std::is_base_of<FilmKeypointLayer, T>::value, "Tracker is based of FilmKeypointLayer");
    //static_assert(!std::is_base_of<FilmKeypointEase, T>::value, "Tracker refuses to be derived from FilmKeypointEase object, the push function is wrong");
    return onPushSetter(dynamic_cast<FilmKeypointLayer*>(keypoint));
}

template<typename T>
inline bool FilmLayerBase::pushTracker(T* keypoint) {
    static_assert(std::is_base_of<FilmKeypointLayer, T>::value, "Tracker is based of FilmKeypointLayer");
    static_assert(!std::is_same<FilmKeypointLayer, T>::value, "Tracker is not a FilmKeypointLayer");
    static_assert(std::is_base_of<FilmKeypointEase, T>::value, "Tracker requires derived from FilmKeypointEase object, the push function is wrong");

    if (!dynamic_cast<FilmKeypointEase*>(keypoint)->ease_func || dynamic_cast<TimerStep*>(keypoint)->is_zero()) return onPushSetter(keypoint);

    Tracker tracker;
    tracker.keypoint = keypoint;

    auto indx = maEases.pushInLocker(tracker);
    if (onPushTracker(indx)) return true;
    return false;
}
