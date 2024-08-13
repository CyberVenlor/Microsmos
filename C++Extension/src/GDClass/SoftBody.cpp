#include "SoftBody.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void SoftBody::_bind_methods(){

}

SoftBody::SoftBody(){
    UtilityFunctions::print("hello physics");
}

SoftBody::~SoftBody(){

}
