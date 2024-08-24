#include "PhysicsObject.h"
#include "GodotAsseccer.h"

Pool<PhysicsObject> PhysicsObject::physics_objects;

PhysicsObject::PhysicsObject(const Ref<BodyData>& body_data, const Vector2& position){
    if (body_data.is_null()) return;
    points = body_data->get_points();
    edges = body_data->get_edges();
    springs = body_data->get_springs();
    for (auto& point : points){
        point.position += position;
    }
}

void PhysicsObject::process(float delta) {
    for_each([delta](PhysicsObject& obj) { obj.point_solver(delta); });
    for_each([](PhysicsObject& obj) { obj.spring_solver(); });
}

void PhysicsObject::draw(){
    for_each([](PhysicsObject& obj) { obj.draw_point(); });
    for_each([](PhysicsObject& obj) { obj.draw_edge(); });
}

void PhysicsObject::draw_point() {
    for (auto& point : points) {
        GodotAsseccer::get_singleton()->draw_circle(point.position, 2, Color(0,0,0,1));
    }
}

void PhysicsObject::draw_edge(){
    for (auto& edge : edges){
        GodotAsseccer::get_singleton()->draw_line(points[edge.p0].position, points[edge.p1].position, Color(0,1,0,1));
    }
}

void PhysicsObject::point_solver(float delta) {
    //UtilityFunctions::print("solve point");
}

void PhysicsObject::spring_solver() {
    //UtilityFunctions::print("solve spring");
}
