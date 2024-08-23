#ifndef INITIALIZER_H
#define INITIALIZER_H

#include <godot_cpp/classes/Json.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <stdio.h>
#include <vector>
#include "Structures.hpp"

namespace godot {
    class BodyData : public Resource {
        GDCLASS(BodyData, Resource)
        //Point
            public:
                void set_file_path(const String& file_path) {
                    this->file_path = file_path;
                }

                String get_file_path() const {
                    return file_path;
                }
                
            protected:
                static void _bind_methods(){
                    ClassDB::bind_method(D_METHOD("set_file_path", "file_path"), &BodyData::set_file_path);
                    ClassDB::bind_method(D_METHOD("get_file_path"), &BodyData::get_file_path);
                    ClassDB::add_property(
                    "BodyData", 
                    PropertyInfo(
                                Variant::STRING, 
                                "FilePath", 
                                PROPERTY_HINT_FILE,
                                "*.body"
                            ), 
                    "set_file_path", 
                    "get_file_path"
                    );
                };
            private:
                String file_path;
    };
    
}

#endif