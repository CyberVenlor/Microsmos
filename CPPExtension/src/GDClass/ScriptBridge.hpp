#ifndef SCRIPTBRIDGE_H
#define SCRIPTBRIDGE_H

#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {
    class Point : RefCounted {
        GDCLASS(Point, RefCounted)
    };
}

#endif