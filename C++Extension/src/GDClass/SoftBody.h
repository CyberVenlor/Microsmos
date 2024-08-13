#ifndef SOFTBODY_H
#define SOFTBODY_H

#include <godot_cpp/classes/node2d.hpp>
#include "PhysicsBody.h"

namespace godot {
    class SoftBody : public PhysicsBody {
        GDCLASS(SoftBody, PhysicsBody)
        public:
            SoftBody();
            ~SoftBody();
        protected:
            static void _bind_methods();
        private:
    };
}

#endif