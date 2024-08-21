#ifndef INITIALIZER_H
#define INITIALIZER_H

#include <godot_cpp/classes/Json.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <stdio.h>

namespace godot {
    class IPoint : public Resource {
        GDCLASS(IPoint, Resource)
        public:
            //position
            void set_position(const Vector2& position) {
                this -> position = position;
            }

            Vector2 get_position() const {
                return position;
            }

            //mass
            void set_mass(const float& mass) {
                this -> mass = mass;
            }

            float get_mass() const {
                return mass;
            }
            
        protected:
            static void _bind_methods(){
                //position
                ClassDB::bind_method(D_METHOD("set_position", "position"), &IPoint::set_position);
                ClassDB::bind_method(D_METHOD("get_position"), &IPoint::get_position);
                ClassDB::add_property("IPoint", PropertyInfo(Variant::VECTOR2, "Position"), "set_position", "get_position");
                //mass
                ClassDB::bind_method(D_METHOD("set_mass", "mass"), &IPoint::set_mass);
                ClassDB::bind_method(D_METHOD("get_mass"), &IPoint::get_mass);
                ClassDB::add_property("IPoint", PropertyInfo(Variant::FLOAT, "Mass"), "set_mass", "get_mass");
            };
        private:
            Vector2 position;
            float mass;
    };
    class BodyData : public Resource {
        GDCLASS(BodyData, Resource)
        public:
            void set_point(const TypedArray<IPoint>& array) {
                points = array;
            }

            TypedArray<IPoint> get_point() const {
                return points;
            }
            
        protected:
            static void _bind_methods(){
                ClassDB::bind_method(D_METHOD("set_point", "array"), &BodyData::set_point);
                ClassDB::bind_method(D_METHOD("get_point"), &BodyData::get_point);
                ClassDB::add_property(
                "BodyData", 
                PropertyInfo(
                            Variant::ARRAY, 
                            "IPoints", 
                            PROPERTY_HINT_TYPE_STRING, 
                            String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":IPoint"
                        ), 
                "set_point", 
                "get_point"
                );
            };
        private:
            TypedArray<IPoint> points;
    };
    
}

#endif