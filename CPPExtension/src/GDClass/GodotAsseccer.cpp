#include "GodotAsseccer.h"
#include "PhysicsObject.h"
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {
    GodotAsseccer* GodotAsseccer::singleton = nullptr;

    GodotAsseccer::GodotAsseccer() {
        if (singleton == nullptr) {
            singleton = this;
        } else {
            UtilityFunctions::print("GodotAsseccer already exists");
        }
    }

    void GodotAsseccer::_physics_process(double delta) {
        PhysicsObject::Process((float)delta);
    }

    void GodotAsseccer::_bind_methods() {}
}