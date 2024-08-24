#ifndef POOL_H
#define POOL_H

#include <vector>
#include <godot_cpp/variant/utility_functions.hpp>
#include <iostream>
#include <iterator>

using namespace std;

template<typename T, bool ignoreEmpty = true>
class Pool {
public:
    // Element access
    inline T& operator[](int index){
        return pool[index];
    }

    // Add element
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

    // Remove element
    inline void remove(int index){
        freeList.push_back(index);
    }

    inline void operator-=(int index){
        remove(index);
    }

    // Get size
    inline int size() const {
        return pool.size();
    }

    // Iterators
    class Iterator {
    public:
        Iterator(typename std::vector<T>::iterator it) : current(it) {}

        T& operator*() const { return *current; }
        T* operator->() const { return &(*current); }

        // Pre-increment
        Iterator& operator++() {
            ++current;
            return *this;
        }

        // Post-increment
        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }

    private:
        typename std::vector<T>::iterator current;
    };

    Iterator begin() {
        return Iterator(pool.begin());
    }

    Iterator end() {
        return Iterator(pool.end());
    }

private:
    std::vector<T> pool;
    std::vector<int> freeList;
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

        inline bool is_active(int index){
            return std::get<1>(pool[index]);
        }
    private:
        std::vector<std::tuple<T,bool>> pool;
        std::vector<int> freeList;
};

#endif