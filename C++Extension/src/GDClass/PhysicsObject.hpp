#ifndef PHYSICS_OBJECT
#define PHYSICS_OBJECT

#include <iostream>
#include "Pool.hpp"
#include "BodyData.hpp"
#include <godot_cpp/variant/vector2.hpp>
#include <functional>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

struct Point {
    Vector2 position;
    Vector2 velocity;
    float mass; 
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

class PhysicsObject {
    public:
        // PhysicsObject(BodyData& bodydata){
        //     //TODO copy bodydata's info
        // }

        static Pool<PhysicsObject> physics_objects;
        static void Process(float delta){
            forEach([delta](PhysicsObject& obj){ obj.pointSolver(delta); });
            forEach([](PhysicsObject& obj){ obj.springSolver(); });
        }
    private:
        Pool<Point> points;
        Pool<Spring> springs;
        Pool<Edge> edges;

        static inline void forEach(const std::function<void(PhysicsObject&)>& func){
            for (int i; i < physics_objects.size(); i++){
                func(physics_objects[i]);
            }
        }

    protected:
        void pointSolver(float delta){
            UtilityFunctions::print("solve point");
        }

        void springSolver(){
            UtilityFunctions::print("solve spring");
        }
};

Pool<PhysicsObject> PhysicsObject::physics_objects;

#endif