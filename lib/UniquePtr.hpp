#pragma once

#include <type_traits>

template <class T>
class UniquePtr {
private:
    using type = std::remove_extent_t<T>;
    type* ptr = nullptr;

    void destroy() {
        if(std::is_array_v<T>){
            delete[] ptr;
        }
        else{
            delete ptr;
        }
    }

public:
    UniquePtr() = default;

    UniquePtr(type* p) : ptr(p) {}

    ~UniquePtr() {
        destroy();
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;
    
    UniquePtr(UniquePtr&& other) : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) {
        if(this != &other){
            destroy();
            ptr = other.ptr;
            other.ptr = nullptr;
        }

        return *this;
    }
    
    type* release() {
        type* result = ptr;
        ptr = nullptr;
        return result;
    }

    void reset(type* p = nullptr) {
        if(ptr != p) {
            destroy();
            ptr = p;
        }
    }

    type& operator*() const requires(!std::is_array_v<T>) {
        return *ptr;
    }

    type* operator->() const requires(!std::is_array_v<T>) {
        return ptr;
    }

    type& operator[](std::size_t index) const requires std::is_array_v<T> {
        return ptr[index];
    }

    type* get() const{
        return ptr;
    }

    operator bool() const {
        return ptr != nullptr;
    }

};
