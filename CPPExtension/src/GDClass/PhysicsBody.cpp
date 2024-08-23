#include "PhysicsBody.h"
#include "PhysicsObject.h"
#include "Pool.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

PhysicsBody::PhysicsBody(){}

PhysicsBody::~PhysicsBody(){}

void PhysicsBody::_enter_tree(){
    //将物理对象放入物理对象池
    physics_object_index = PhysicsObject::physics_objects += PhysicsObject();
}

void PhysicsBody::_exit_tree(){
    //将物理对象移除
    PhysicsObject::physics_objects -= physics_object_index;
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