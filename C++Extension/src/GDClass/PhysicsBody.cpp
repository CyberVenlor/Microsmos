#include "PhysicsBody.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

PhysicsBody::PhysicsBody(){

}

PhysicsBody::~PhysicsBody(){

}

void PhysicsBody::_bind_methods(){
    ClassDB::bind_method(D_METHOD("get_bodyData"), &PhysicsBody::getBodyData);
    ClassDB::bind_method(D_METHOD("set_bodyData", "bodyData"),&PhysicsBody::setBodyData);
    ClassDB::add_property(
        "PhysicsBody", 
        PropertyInfo(
            Variant::OBJECT,
            "BodyData",
            PROPERTY_HINT_RESOURCE_TYPE,
            "BodyData"
        ),
        "set_bodyData",
        "get_bodyData"
    );
}


Ref<BodyData> PhysicsBody::getBodyData() const { 
    return bodyData;
}

void PhysicsBody::setBodyData(const Ref<BodyData> bodyData){
    this->bodyData = bodyData;
}