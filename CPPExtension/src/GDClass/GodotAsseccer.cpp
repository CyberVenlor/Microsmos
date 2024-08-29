#include "GodotAsseccer.hpp"
#include "PhysicsObject.hpp"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/input.hpp>

namespace godot {
    GodotAsseccer* GodotAsseccer::singleton = nullptr;

    GodotAsseccer::GodotAsseccer(){
        if (singleton == nullptr) {
            singleton = this;
        } else {
            UtilityFunctions::print("GodotAsseccer already exists");
        }
    }

    void GodotAsseccer::_physics_process(double delta){
        //PhysicsObject::process(delta);
        if (Input::get_singleton()->is_action_just_pressed("ui_accept")){
            PhysicsObject::process(delta);
        }
        if (Input::get_singleton()->is_action_pressed("ui_right")){
            PhysicsObject::process(delta);
        }
    }

    void GodotAsseccer::_process(double delta){
        queue_redraw();
    }

    void GodotAsseccer::_draw(){
        PhysicsObject::draw();
    }

    void GodotAsseccer::_bind_methods(){}

    GodotAsseccer* GodotAsseccer::get_singleton(){
        return singleton;
    }
}