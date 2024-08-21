#ifndef SINGLETON_H
#define SINGLETON_H

#include <godot_cpp/classes/node2D.hpp>

namespace godot{
    class Singleton : public Node2D {
        GDCLASS(Singleton, Node2D);
    };
}

#endif