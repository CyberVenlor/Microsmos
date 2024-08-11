#include "HelloWorld.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void HelloWorld::_bind_methods(){

}

HelloWorld::HelloWorld(){
    helloWorld();
}

HelloWorld::~HelloWorld(){

}

void HelloWorld::helloWorld(){
    UtilityFunctions::print("hello world");
}