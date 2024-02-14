#pragma once

#include <iostream>

#include "utils/math_utils.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"


inline glm::vec3 NormalizedRGB(unsigned char r, unsigned char g, unsigned char b) {
    return glm::vec3(r / 255.f, g / 255.f, b / 255.f);
}

/**
 *  overload ostream operator << for easy printing glm classes
 */
inline std::ostream &operator<< (std::ostream &out, const glm::ivec2 &vec) {
    out << "[" << vec.x << " " << vec.y << "]";
    return out;
}

inline std::ostream &operator<< (std::ostream &out, const glm::vec2 &vec) {
    out << "[" << vec.x << " " << vec.y << "]";
    return out;
}

inline std::ostream &operator<< (std::ostream &out, const glm::ivec3 &vec) {
    out << "[" << vec.x << " " << vec.y << " " << vec.z << "]";
    return out;
}

inline std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec) {
    out << "[" << vec.x << " " << vec.y << " " << vec.z << "]";
    return out;
}

inline std::ostream &operator<< (std::ostream &out, const glm::ivec4 &vec) {
    out << "[" << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << "]";
    return out;
}

inline std::ostream &operator<< (std::ostream &out, const glm::vec4 &vec) {
    out << "[" << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << "]";
    return out;
}

inline std::ostream &operator<< (std::ostream &out, const glm::quat &rot) {
    out << "[" << rot.x << " " << rot.y << " " << rot.z << " " << rot.w << "]";
    return out;
}

inline std::ostream& operator<< (std::ostream &out, const glm::mat4 &mat) {
    return out << "[" << mat[0] << " " << mat[1] << " " << mat[2] << " " << mat[3] << "]";
}

#ifndef extra_glm_utils
#define extra_glm_utils

namespace glm
{
    /*
        computes distance between point "point" and semiline defined by point "start" and direction "dir"
        returns negative distance if "point" is behind semiline
    */
    inline float dist2line(glm::vec3 point, glm::vec3 start, glm::vec3 dir) {
        glm::vec3 hyp = point - start;
        float lenh = sqrtf(glm::length2(hyp));
        float prodm = sqrtf(glm::length2(dir)) * lenh;
        float sin = sqrtf(glm::length2(glm::cross(hyp, dir))) / prodm;
        float cos = glm::dot(hyp, dir) / prodm;
        float sign = cos < 0.0f ? -1.0f : 1.0f;
        float dist = lenh * abs(sin);
        return dist * sign;
    }

    const float FEPS = 0.00001f;

    // computes slope between 2 points
    inline float slope(vec2 p1, vec2 p2) {
        return abs(p1.x - p2.x) < FEPS ?
            p1.x < p2.x ? 1.0f / FEPS : -1.0f / FEPS :
            (p1.y - p2.y) / (p1.x - p2.x);
    }

    inline bool compare(vec2 v1, vec2 v2) { return v1.x == v2.x ? v1.y < v2.y : v1.x < v2.x; }

    inline bool compare2D(vec3 v1, vec3 v2) { return v1.x == v2.x ? v1.y == v2.y ? v1.z < v2.z : v1.y < v2.y : v1.x < v2.x; }

    // rotates a 2D point v with a slope stored in the Z coordinate by an angle a
    inline vec3 rotate(vec3 v, float a) {
        return vec3(v.x * cosf(a) - v.y * sinf(a),
            v.x * sinf(a) + v.y * cosf(a),
            abs(v.z + tanf(a)) < FEPS ? (v.z + tanf(a)) / (1 - v.z * tanf(a)) :
                (abs((1 - v.z * tanf(a)) / (v.z + tanf(a))) < FEPS ?
                    ((1 - v.z * tanf(a)) / (v.z + tanf(a)) > 0 ? 1.0f / FEPS : -1.0f / FEPS) :
                    (v.z + tanf(a)) / (1.0f - tanf(a) * v.z)));
    }

    inline mat4 subscale(mat4 m) {
        mat4 tm = transpose(m);
        float l1 = length(vec3(tm * vec4(1.0f, 0.0f, 0.0f, 0.0f)));
        float l2 = length(vec3(tm * vec4(0.0f, 1.0f, 0.0f, 0.0f)));
        float l3 = length(vec3(tm * vec4(0.0f, 0.0f, 1.0f, 0.0f)));
        return scale(mat4(1.0f), vec3(l1, l2, l3));
    }

    inline mat4 subrotation(mat4 m) {
        mat4 ssm = subscale(m);
        if (determinant(ssm) == 0) return m;
        else return translate(m, -vec3(m * vec4(0.0f, 0.0f, 0.0f, 1.0f))) * inverse(ssm);
    }

    const mat4 um4 = mat4(1.0f);
    const vec3 dirz = vec3(0.0f, 0.0f, 1.0f);
    const vec3 diry = vec3(0.0f, 1.0f, 0.0f);
    const vec3 dirx = vec3(1.0f, 0.0f, 0.0f);

    using namespace std;

    const glm::vec3 vec3_up = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::vec3 vec3_down = glm::vec3(0.0f, -1.0f, 0.0f);
    const glm::vec3 vec3_left = glm::vec3(-1.0f, 0.0f, 0.0f);
    const glm::vec3 vec3_right = glm::vec3(1.0f, 0.0f, 0.0f);
    const glm::vec3 vec3_forward = glm::vec3(0.0f, 0.0f, 1.0f);
    const glm::vec3 vec3_backward = glm::vec3(0.0f, 0.0f, -1.0f);

    inline glm::quat AxisAngle(const float xx, const float yy, const float zz, const float angle360)
    {
        float t = float(RADIANS(angle360) / 2.0f);

        // Precompute the sin(theta/2) once for optimization
        float sinT = sin(t);

        // Calculate the x, y and z of the quaternion
        float x = xx * sinT;
        float y = yy * sinT;
        float z = zz * sinT;
        float w = cos(t);        // W value by cos(theta/2)

        return glm::quat(w, x, y, z);
    }

    // Convert a quaternion to axis angle
    inline glm::vec4 GetAxisAngle(glm::quat &rotation, int precision = 0)
    {
        float angle = acos(rotation.w);

        if (angle == 0)
        {
            return glm::vec4(1, 0, 0, 0);
        }

        auto t = sqrt(1 - rotation.w * rotation.w);

        if (precision)
        {
            auto x = round(rotation.x / t * precision) / precision;
            auto y = round(rotation.y / t * precision) / precision;
            auto z = round(rotation.z / t * precision) / precision;
            return glm::vec4(x, y, z, round(DEGREES(angle)));
        }

        return glm::vec4(rotation.x / t, rotation.y / t, rotation.z / t, round(DEGREES(angle)));
    }
}

#endif