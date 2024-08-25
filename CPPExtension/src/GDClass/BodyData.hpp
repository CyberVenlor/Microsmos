#ifndef INITIALIZER_H
#define INITIALIZER_H

#include <godot_cpp/classes/Json.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <stdio.h>
#include <vector>
#include "Pool.hpp"
#include "Structures.hpp"
#include "TypeDefine.hpp"

namespace godot {
    class BodyData : public Resource {
        GDCLASS(BodyData, Resource)
        //Point
        public:
            void set_file_path(const String& file_path) {
                this->file_path = file_path;
                read_binary();
            }

            String get_file_path() const {
                return file_path;
            }

            const Pool<Point>& get_points() const { return points; }
            const Pool<Edge>& get_edges() const { return edges; }
            const Pool<Spring>& get_springs() const { return springs; }
            
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

            Pool<Point> points;
            Pool<Edge> edges;
            Pool<Spring> springs;

            void read_binary(){//他奶奶的不是按照参数顺序读取
                if (file_path.get_extension() != "body") return;
                Ref<FileAccess> file = FileAccess::open(file_path, FileAccess::ModeFlags::READ);
                // //Point
                u32 point_count = file->get_32();
                for (u32 i = 0; i < point_count; i++){
                    f32 x = file->get_float();
                    f32 y = file->get_float();
                    points += Point(Vector2(x, y), file->get_float());
                }
                // //Edge
                u32 edge_count = file->get_32();
                for (u32 i = 0; i < edge_count; i++){
                    u32 p0 = file->get_32();
                    u32 p1 = file->get_32();
                    edges += Edge(p0, p1, file->get_float());
                }
                // //Spring
                u32 spring_count = file->get_32();
                for (u32 i = 0; i < spring_count; i++){
                    u32 p0 = file->get_32();
                    u32 p1 = file->get_32();
                    f32 strength = file->get_float();
                    springs += Spring(p0, p1, 50, file->get_float());
                }
            }
    };
    
}

#endif