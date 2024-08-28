#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <godot_cpp/variant/vector2.hpp>
#include "TypeDefine.hpp"

using namespace godot;

// struct Vec2 {
//     f64 x = 0;
//     f64 y = 0;

//     Vec2(){}
//     Vec2(const Vec2& vec2) : x(vec2.x), y(vec2.y) {}
//     Vec2(f64 x, f64 y) : x(x), y(y) {}
//     Vec2(const Vector2& vector) : x(vector.x), y(vector.y) {}

//     inline Vec2 operator+(const Vec2 &other) const {
//         return Vec2(x + other.x, y + other.y);
//     }

//     inline void operator+=(const Vec2 &other) {
//         x += other.x;
//         y += other.y;
//     }

//     inline Vec2 operator-(const Vec2 &other) const {
//         return Vec2(x - other.x, y - other.y);
//     }

//     inline void operator-=(const Vec2 &other) {
//         x -= other.x;
//         y -= other.y;
//     }

//     inline Vec2 operator*(const Vec2 &other) const {
//         return Vec2(x * other.x, y * other.y);
//     }

//     inline Vec2 operator*(const f64 &value) const {
//         return Vec2(x * value, y * value);
//     }

//     inline void operator*=(const f64 &value) {
//         x *= value;
//         y *= value;
//     }

//     inline Vec2 operator/(const Vec2 &other) const {
//         return Vec2(x / other.x, y / other.y);
//     }

//     inline Vec2 operator/(const f64 &value) const {
//         return Vec2(x / value, y / value);
//     }

//     inline void operator/=(const f64 &value) {
//         x /= value;
//         y /= value;
//     }

//     inline Vec2 operator-() const {
//         return Vec2(-x, -y);
//     }

//     inline bool operator==(const Vec2 &other) const {
//         return x == other.x && y == other.y;
//     }

//     inline bool operator!=(const Vec2 &other) const {
//         return x != other.x || y != other.y;
//     }

//     operator Vector2() const {
//         return Vector2(x, y);
//     }

//     f64 dot(const Vec2 &other) const {
// 	    return x * other.x + y * other.y;
//     }

//     f64 distance_to(const Vec2 &other) const {
// 	    return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
//     }

//     Vec2 normalized() {
//         f64 l = x * x + y * y;
//         if (l != 0) {
//             l = sqrt(l);
//             return Vec2(
//                 x / l,
//                 y / l
//             );
//         }
//         return Vec2(0,0);
//     }
// };

struct Point {
    Vector2 position;
    Vector2 velocity;
    Vector2 force;
    f32 mass; 

    Point(const Vector2& position, f32 mass) : position(position), mass(mass){}
};

struct Edge {
    u32 p0;
    u32 p1;
    f32 reflection;

    Edge(u32 p0, u32 p1, f32 reflection) : p0(p0), p1(p1), reflection(reflection){}
};

struct Spring {
    u32 p0;
    u32 p1;
    f32 length;
    f32 strength;
    f32 damping;

    Spring(u32 p0, u32 p1, f32 strength, f32 damping) : p0(p0), p1(p1), strength(strength), damping(damping){}
};

#endif