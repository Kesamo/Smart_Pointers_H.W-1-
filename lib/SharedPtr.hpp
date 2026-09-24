#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

template <typename T>
struct ControlBlock {
    using type = std::remove_extent_t<T>;

    std::size_t counter = 1;
    type* ptr = nullptr;

    ControlBlock(type* pointer) noexcept : ptr(pointer) {}

    ~ControlBlock() {
        if(std::is_array_v<T>) {
            delete[] ptr;
        } else {
            delete ptr;
        }
    }
};

template <typename T>
class SharedPtr {
private:
    using type = std::remove_extent_t<T>;

    ControlBlock<T>* control = nullptr;

    void release_control() noexcept {
        if(control == nullptr) {
            return;
        }

        --control->counter;

        if(control->counter == 0) {
            delete control;
        }

        control = nullptr;
    }

public:
    SharedPtr() noexcept = default;

    SharedPtr(type* ptr) {
        if(ptr == nullptr) {
            return;
        }

        try {
            control = new ControlBlock<T>(ptr);
        } 
        catch (...) {
            if(std::is_array_v<T>) {
                delete[] ptr;
            } else {
                delete ptr;
            }

            throw;
        }
    }

    SharedPtr(const SharedPtr& other) noexcept : control(other.control) {
        if(control) {
            control->counter++;
        }
    }

    SharedPtr(SharedPtr&& other) noexcept : control(other.control) {
        other.control = nullptr;
    }

    ~SharedPtr() {
        release_control();
    }

    SharedPtr& operator=(const SharedPtr& other) noexcept {
        if(this == &other) {
            return *this;
        }

        ControlBlock<T>* new_control = other.control;

        if(new_control != nullptr) {
            ++new_control->counter;
        }

        release_control();

        control = new_control;

        return *this;
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if(this == &other) {
            return *this;
        }

        release_control();

        control = other.control;
        other.control = nullptr;

        return *this;
    }

    void reset(type* ptr = nullptr) {
        if(get() == ptr) {
            return;
        }

        SharedPtr temporary(ptr);

        release_control();

        control = temporary.control;
        temporary.control = nullptr;
    }

    type* get() const noexcept {
        if(control == nullptr) {
            return nullptr;
        }

        return control->ptr;
    }

    std::size_t use_count() const noexcept {
        if(control == nullptr) {
            return 0;
        }

        return control->counter;
    }

    type& operator*() const requires (!std::is_array_v<T>) {
        return *control->ptr;
    }

    type* operator->() const noexcept requires (!std::is_array_v<T>) {
        return control->ptr;
    }

    type& operator[](std::size_t index) const requires std::is_array_v<T> {
        return control->ptr[index];
    }

    operator bool() const noexcept {
        return get() != nullptr;
    }
};