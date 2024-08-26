#ifndef PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H

#include "Pool.hpp"
#include "BodyData.hpp"
#include "Structures.hpp"
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <functional>

using namespace godot;

class PhysicsObject {
    public:
        PhysicsObject(const Ref<BodyData>& body_data, const Vector2& position);
        static Pool<PhysicsObject> physics_objects;
        static void process(float delta);
        static void draw();
    private:
        Pool<Point> points;
        Pool<Edge> edges;
        Pool<Spring> springs;

        static void for_each_thread(const std::function<void(PhysicsObject&)>& func);
        static void for_each(const std::function<void(PhysicsObject&)>& func);

    protected:
        void draw_point();
        void draw_edge();
        void draw_spring();
        void solve_force(f32 delta);
        void solve_point(f32 delta);
        void solve_spring(f32 delta);
        void solve_edge(f32 delta);
        f32 continuous_collision_detection(const Point& point, const Point& p0, const Point& p1, f32 delta);
};

#endif