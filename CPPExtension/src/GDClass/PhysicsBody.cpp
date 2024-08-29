#include "PhysicsBody.hpp"
#include "PhysicsObject.hpp"
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
    physics_object_index = PhysicsObject::physics_objects += PhysicsObject(body_data, Vector2(0,0));
}

void PhysicsBody::_exit_tree(){
    //将物理对象移除
    PhysicsObject::physics_objects -= physics_object_index;
}

void PhysicsBody::_bind_methods(){
    ClassDB::bind_method(D_METHOD("get_body_data"), &PhysicsBody::get_body_data);
    ClassDB::bind_method(D_METHOD("set_body_data", "body_data"),&PhysicsBody::set_body_data);
    ClassDB::add_property(
        "PhysicsBody", 
        PropertyInfo(
            Variant::OBJECT,
            "Data",
            PROPERTY_HINT_RESOURCE_TYPE,
            "BodyData"
        ),
        "set_body_data",
        "get_body_data"
    );
}


Ref<BodyData> PhysicsBody::get_body_data() const { 
    return body_data;
}

void PhysicsBody::set_body_data(const Ref<BodyData> bodyData){
    this->body_data = bodyData;
}