#ifndef INITIALIZER_H
#define INITIALIZER_H

#include <godot_cpp/classes/Json.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <stdio.h>
#include "Pool.hpp"

namespace godot {

    class BodyData : public JSON {
        GDCLASS(BodyData, JSON)
        public:
            BodyData(){
                UtilityFunctions::print("/////////");
                Pool<int, true> a = Pool<int, true>();
                for (int i; i < 10e4; i++){
                    a += 1;
                }

                a -= 1;
                UtilityFunctions::print("/",a += 5,"/");
                a += 6;
                

                UtilityFunctions::print(a[0], a[1], a[2]);
            }
            ~BodyData(){};
        protected:
            static void _bind_methods(){};
        private:
    };
}

#endif