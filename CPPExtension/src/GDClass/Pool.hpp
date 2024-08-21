#ifndef POOL_H
#define POOL_H

#include <vector>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace std;

template<typename T, bool ignoreEmpty = true>
class Pool {
    public:
        inline T& operator[](int index){
            return pool[index];
        }
        inline int add(T&& element){
            if (freeList.empty()){
                pool.push_back(forward<T>(element));
                return pool.size() - 1;
            } else {
                const int back = freeList.back();
                pool[back] = forward<T>(element);
                freeList.pop_back();
                return back;
            }
        }

        inline int operator+=(T&& element){
            return add(forward<T>(element));
        }

        inline void remove(int index){
            freeList.push_back(index);
        }

        inline void operator-=(int index){
            remove(index);
        }

        inline int size(){
            return pool.size();
        }
    private:
        vector<T> pool;
        vector<int> freeList;
};

template<typename T>
class Pool<T, false> {
    public:
        inline T& operator[](int index){
            return std::get<0>(pool[index]);
        }
        inline int add(T&& element){
            if (freeList.empty()) {
                pool.push_back(std::make_tuple(std::forward<T>(element), true));
                return pool.size() - 1;
            }
            else {
                const int back = freeList.back();
                pool[back] = std::make_tuple(std::forward<T>(element), true);
                freeList.pop_back();
                return back;
            }
        }

        inline int operator+=(T&& element){
            return add(std::forward<T>(element));
        }

        inline void remove(int index){
            freeList.push_back(index);
            std::get<1>(pool[index]) = false;
        }

        inline void operator-=(int index){
            remove(index);
        }

        inline bool isActive(int index){
            return std::get<1>(pool[index]);
        }
    private:
        std::vector<std::tuple<T,bool>> pool;
        std::vector<int> freeList;
};

#endif