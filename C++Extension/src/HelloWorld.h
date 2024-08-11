#ifndef HELLOWORLD_H
#define HELLOWORLD_H

#include <godot_cpp/classes/node2d.hpp>

namespace godot {
    class HelloWorld : public Node2D {
        GDCLASS(HelloWorld, Node2D)

        private:
            int a = 0;
        
        protected:
            static void _bind_methods();
        public:
            HelloWorld();
            ~HelloWorld();

            void helloWorld();
    };
}

#endif