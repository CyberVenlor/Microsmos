#ifndef PHYSICSBODY_H
#define PHYSICSBODY_H

#include <godot_cpp/classes/node.hpp>
#include "BodyData.hpp"

namespace godot {
    class PhysicsBody : public Node {
        GDCLASS(PhysicsBody, Node)
        public:
            PhysicsBody();
            ~PhysicsBody();
            //attribute bodydata
            Ref<BodyData> getBodyData() const;
            void setBodyData (const Ref<BodyData> bodyData);

            void _enter_tree() override;
            void _exit_tree() override;
            
        protected:
            static void _bind_methods();
            
        private:
            Ref<BodyData> bodyData;
            int physics_object_index;
    };
}

#endif