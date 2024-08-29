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
            Ref<BodyData> get_body_data() const;
            void set_body_data (const Ref<BodyData> body_Data);

            void _enter_tree() override;
            void _exit_tree() override;
            
        protected:
            static void _bind_methods();
            
        private:
            Ref<BodyData> body_data;
            int physics_object_index;
    };
}

#endif