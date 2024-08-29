#ifndef GODOTASSECCER_H
#define GODOTASSECCER_H

#include <godot_cpp/classes/node2D.hpp>


namespace godot {
    class GodotAsseccer : public Node2D {
        GDCLASS(GodotAsseccer, Node2D)
        public:
            GodotAsseccer();

            static GodotAsseccer* get_singleton();

            void _physics_process(double delta) override;
            void _process(double delta) override;
            void _draw() override;
        protected:
            static void _bind_methods();
        private:
            static GodotAsseccer* singleton;
    };
}

#endif