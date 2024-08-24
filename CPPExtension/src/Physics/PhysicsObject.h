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
        PhysicsObject(const Ref<BodyData>& body_data);
        PhysicsObject(const Pool<Point>& points, const Pool<Edge>& edges, const Pool<Spring>& springs) : points(points), edges(edges), springs(springs) {}
        static Pool<PhysicsObject> physics_objects;
        static void process(float delta);
        static void draw();
    private:
        Pool<Point> points;
        Pool<Edge> edges;
        Pool<Spring> springs;

        static inline void for_each(const std::function<void(PhysicsObject&)>& func) {
            for (auto& physics_object : physics_objects) {
                func(physics_object);
            }
        }

    protected:
        void draw_point();
        void point_solver(float delta);
        void spring_solver();
};

#endif