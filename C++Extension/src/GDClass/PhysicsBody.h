#ifndef PHYSICSBODY_H
#define PHYSICSBODY_H

#include <godot_cpp/classes/node2d.hpp>
#include "BodyData.hpp"

namespace godot {
    class PhysicsBody : public Node {
        GDCLASS(PhysicsBody, Node)
        public:
            PhysicsBody();
            ~PhysicsBody();
            Ref<BodyData> getBodyData() const;
            void setBodyData (const Ref<BodyData> bodyData);
        protected:
            static void _bind_methods();
        private:
            Ref<BodyData> bodyData;
            
    };
}

#endif