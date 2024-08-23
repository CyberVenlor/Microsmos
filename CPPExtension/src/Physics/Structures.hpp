#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <godot_cpp/variant/vector2.hpp>

using namespace godot;

struct Point {
    Vector2 position;
    Vector2 velocity;
    float mass; 

    Point(const Vector2& position, float mass) : position(position), mass(mass){}
};

struct Spring {
    int p0;
    int p1;
    float strength;
    float damping;
};

struct Edge {
    int p0;
    int p1;
    float reflection;
};


#endif