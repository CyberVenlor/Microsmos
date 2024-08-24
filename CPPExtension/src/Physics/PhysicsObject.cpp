#include "PhysicsObject.h"
#include "GodotAsseccer.h"

Pool<PhysicsObject> PhysicsObject::physics_objects;

PhysicsObject::PhysicsObject(const Ref<BodyData>& body_data){
    points = body_data->get_points();
    edges = body_data->get_edges();
    springs = body_data->get_springs();
}

void PhysicsObject::process(float delta) {
    for_each([delta](PhysicsObject& obj) { obj.point_solver(delta); });
    for_each([](PhysicsObject& obj) { obj.spring_solver(); });
}

void PhysicsObject::draw(){
    for_each([](PhysicsObject& obj) { obj.draw_point(); });
}

void PhysicsObject::draw_point() {
    for (int i = 0; i < points.size(); i++) {
        GodotAsseccer::get_singleton()->draw_circle(points[i].position, 2, Color(0,0,0,1));
    }
}

void PhysicsObject::point_solver(float delta) {
    //UtilityFunctions::print("solve point");
}

void PhysicsObject::spring_solver() {
    //UtilityFunctions::print("solve spring");
}
