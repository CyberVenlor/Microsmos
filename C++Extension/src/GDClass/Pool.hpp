#ifndef POOL_H
#define POOL_H

#include <vector>
#include <godot_cpp/variant/utility_functions.hpp>

template<typename T, bool ignoreEmpty>
class Pool {
    public:
        inline T& operator[](int index){
            return pool[index];
        }
        inline int add(T&& element){
            if (freeList.empty()){
                pool.push_back(std::forward<T>(element));
                return pool.size() - 1;
            } else {
                const int back = freeList.back();
                pool[back] = std::forward<T>(element);
                freeList.pop_back();
                return back;
            }
        }

        inline int operator+=(T&& element){
            return add(std::forward<T>(element));
        }

        inline void remove(int index){
            freeList.push_back(index);
        }

        inline void operator-=(int index){
            remove(index);
        }
    private:
        std::vector<T> pool;
        std::vector<int> freeList;
};

// template<bool ignoreEmpty>
// class Pool {
//     public:
//         inline T& operator[](int index){
//             return std::get<0>(pool[index]);
//         }
//         inline void add(T&& element){
//             if (freeList.empty()) pool.push_back(std::make_tuple(std::move(element), true));
//             else {
//                 freeList[freeList.back()] = std::make_tuple(std::move(element), true);
//                 freeList.pop_back();
//             }
//         }

//         inline void operator+=(T&& element){
//             add(element);
//         }

//         inline void remove(int index){
//             freeList.push_back(index);
//         }

//         inline void operator-=(int index){
//             remove(index);
//         }

//         inline bool isActive(int index){
//             return std::get<1>(pool[index]);
//         }
//     private:
//         std::vector<std::tuple<T,bool>> pool;
//         std::vector<int> freeList;
// };

#endif