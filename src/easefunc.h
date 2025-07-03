#ifndef EASEFUNC_H_
#define EASEFUNC_H_

    #ifdef __cplusplus
    extern "C" {
    #endif

    #ifndef USE_THIRD_PARTY_MATH
    #include <math.h>

        #ifndef M_PI
        #define M_PI 3.14159265358979323846
        #endif

        #ifndef M_PI_F
        #define M_PI_F 3.14159265358979323846f
        #endif

    #endif

inline float lerpex(float x, float x0, float y0, float x1, float y1) {
    return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}

inline float lerp(float x, float v0, float v1) {
    return v0 + x * (v1 - v0);
}

inline float ease_linear(float x) {
    return x;
}

inline float ease_quad_in(float x) {
    return x * x;
}

inline float ease_quad_out(float x) {
    return -(x - 1) * (x - 1) + 1;
}

inline float ease_quad_in_out(float x) {
    return x < 0.5f ? x * x * 2.f : -2.f * (x - 1.f) * (x - 1.f) + 1.f;
}

inline float ease_cubic_in(float x) {
    return x * x * x;
}

inline float ease_cubic_out(float x) {
    float t = x - 1.f;
    return t * t * t + 1;
}

inline float ease_cubic_in_out(float x) {
    float t = x < 0.5f ? 2.f * x : 2.f * x - 2.f;
    return 0.5f * t * t * t + float(x >= 0.5f);
}

inline float ease_back_in(float x) {
    return (x - 0.5f) * x * (x + 1.f);
}

inline float ease_back_out(float x) {
    return 1.f - (0.5f - x) * (1.f - x) * (2.f - x);
}

inline float ease_back_in_out(float x) {
    float t = x < 0.5f ? 2.f * x : 2.f * x - 1.f;
    return x < 0.5f
        ? 0.5f * (t - 0.5f) * t * (t + 1.f)
        : 0.5f + 0.5f * (1.f - (1.5f - t) * (1.f - t) * (2.f - t));
}

    #ifndef USE_THIRD_PARTY_MATH
    inline float ease_sine_in(float x) {
        return 1.f - cosf((x * M_PI_F) / 2.f);
    }

    inline float ease_sine_out(float x) {
        return sinf((x * M_PI_F) / 2.f);
    }

    inline float ease_sine_in_out(float x) {
        return -0.5f * cosf(M_PI_F * x) + 0.5f;
    }
    #endif

    #ifdef __cplusplus
    }
    #endif

#endif