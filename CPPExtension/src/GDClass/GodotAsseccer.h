#ifndef GODOTASSECCER_H
#define GODOTASSECCER_H

#include <godot_cpp/classes/node2D.hpp>


namespace godot {
    class GodotAsseccer : public Node2D {
        GDCLASS(GodotAsseccer, Node2D)
        public:
            GodotAsseccer();

            inline static GodotAsseccer* getSingleton();

            void _physics_process(double delta) override;
        protected:
            static void _bind_methods();
        private:
            static GodotAsseccer* singleton;
    };

    // Inline definition of getSingleton()
    inline GodotAsseccer* GodotAsseccer::getSingleton() {
        return singleton;
    }
}

#endif