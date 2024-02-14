#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdlib>

template<class T> T lerp(T v0, T v1, float t)
{
    return v0 + (v1 - v0) * t;
}

/*
    Logarithmically lerps between a and b,
    with factor dt and exponentiation function f
*/
template<class T> T loglerp(T a, T b, float dt, T(*pp)(T, float)) {
    return a * pp(b / a, dt);
}

/*
    generates random positive signed int, period
    uses rand() function
    i.e srand() before calling this
*/

inline unsigned int randui() {
    unsigned int x = rand();
    x = (x << 15) + rand();
    return (x << 2) + rand() % 4;
}

/*
    generates random float in math interval [0, 1);
    uses rand() function
    i.e srand() before calling this
*/
inline float randf() {
    unsigned int x = randui() % 0x80000000;
    unsigned int m = 0x80000000;
    double r = (double) x / (double) m;
    return (float) r;
}


/*
    generates random float in math interval [a, b)
    uses rand() function
    i.e. srand() before calling this
    if you want (a, b] just call with parameters reversed
*/
inline float randf(float a, float b) { return lerp(a, b, randf()); }

#ifndef MAX
#   define MAX(a, b)        (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#   define MIN(a, b)        (((a) < (b)) ? (a) : (b))
#endif

#define UPPER_BOUND(a, b)   (((a) + (b) - 1) / (b))

#define RADIANS(angle)      ((angle) * 0.0174532925194444f)     // a * M_PI / 180
#define TO_RADIANS          0.0174532925194444f                 // M_PI / 180

#define DEGREES(radians)    ((radians) * 57.29577951308233f)
#define TO_DEGREES          57.29577951308233f                  // 180 / M_PI
