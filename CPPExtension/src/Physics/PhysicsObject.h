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
        static Pool<PhysicsObject> physics_objects;
        static void Process(float delta);
    private:
        Pool<Point> points;
        Pool<Spring> springs;
        Pool<Edge> edges;

        static inline void forEach(const std::function<void(PhysicsObject&)>& func) {
            for (int i = 0; i < physics_objects.size(); i++) {
                func(physics_objects[i]);
            }
        }

    protected:
        void drawPoint();
        void pointSolver(float delta);
        void springSolver();
};

#endif