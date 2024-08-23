#include "PhysicsObject.h"
#include "GodotAsseccer.h"

Pool<PhysicsObject> PhysicsObject::physics_objects;

void PhysicsObject::Process(float delta) {
    forEach([](PhysicsObject& obj) { obj.drawPoint(); });
    forEach([delta](PhysicsObject& obj) { obj.pointSolver(delta); });
    forEach([](PhysicsObject& obj) { obj.springSolver(); });
}

void PhysicsObject::drawPoint() {
    for (int i = 0; i < points.size(); i++) {
        GodotAsseccer::getSingleton()->draw_circle(points[i].position, points[i].mass, Color(1,0,0,1));
    }
}

void PhysicsObject::pointSolver(float delta) {
    UtilityFunctions::print("solve point");
}

void PhysicsObject::springSolver() {
    UtilityFunctions::print("solve spring");
}
