#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <godot_cpp/variant/vector2.hpp>
#include "TypeDefine.hpp"

using namespace godot;

struct Point {
    Vector2 position;
    Vector2 velocity;
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