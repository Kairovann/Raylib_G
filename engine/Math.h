#pragma once
#include <cmath>

struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;
};

inline Vec2 operator+(Vec2 a, Vec2 b) { return {a.x + b.x, a.y + b.y}; }
inline Vec2 operator-(Vec2 a, Vec2 b) { return {a.x - b.x, a.y - b.y}; }
inline Vec2 operator*(Vec2 a, float s) { return {a.x * s, a.y * s}; }

inline float Dot(Vec2 a, Vec2 b) { return a.x * b.x + a.y * b.y; }
inline float LengthSqr(Vec2 a) { return a.x * a.x + a.y * a.y; }
inline float Length(Vec2 a) { return std::sqrt(LengthSqr(a)); }
inline float Distance(Vec2 a, Vec2 b) { return Length(a - b); }

inline Vec2 Normalize(Vec2 a) {
    float len = Length(a);
    if (len < 1e-8f) return {0.0f, 0.0f};
    return {a.x / len, a.y / len};
}

inline Vec2 Lerp(Vec2 a, Vec2 b, float t) { return a + (b - a) * t; }       // Linear Interpolation
