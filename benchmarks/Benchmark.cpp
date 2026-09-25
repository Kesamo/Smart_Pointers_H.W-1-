#include "Benchmark.hpp"
#include "SharedPtr.hpp"
#include "UniquePtr.hpp"

#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string_view>
#include <utility>

using Clock = std::chrono::steady_clock;

constexpr std::size_t iterations = 500'000;

template<typename T>
inline void escape(T& value) noexcept {
    asm volatile("" : : "g"(std::addressof(value)) : "memory");
}

template <typename Function>
double measure(Function operation) {
    const auto begin = Clock::now();

    for(std::size_t i = 0; i < iterations; ++i) {
        operation();
    }

    const auto end = Clock::now();

    const double nanoseconds =
        std::chrono::duration<double, std::nano>(
            end - begin
        ).count();

    return nanoseconds /
           static_cast<double>(iterations);
}

void print_result(std::string_view name, double nanoseconds) {
    std::cout
        << std::left
        << std::setw(25)
        << name
        << std::right
        << std::setw(12)
        << nanoseconds
        << " ns/op\n";
}



double raw_create() {
    return measure([] {
        int* pointer = new int(42);

        escape(pointer);

        delete pointer;
    });
}

double custom_unique_create() {
    return measure([] {
        UniquePtr<int> pointer(new int(42));

        escape(pointer);
    });
}

double standard_unique_create() {
    return measure([] {
        std::unique_ptr<int> pointer(new int(42));

        escape(pointer);
    });
}

double custom_shared_create() {
    return measure([] {
        SharedPtr<int> pointer(new int(42));

        escape(pointer);
    });
}

double standard_shared_create() {
    return measure([] {
        std::shared_ptr<int> pointer(new int(42));

        escape(pointer);
    });
}


double raw_dereference() {
    int* pointer = new int(42);

    const double result = measure([&] {
        int value = *pointer;

        escape(value);
        escape(pointer);
    });

    delete pointer;

    return result;
}

double custom_unique_dereference() {
    UniquePtr<int> pointer(new int(42));

    return measure([&] {
        int value = *pointer;

        escape(value);
        escape(pointer);
    });
}

double standard_unique_dereference() {
    std::unique_ptr<int> pointer(new int(42));

    return measure([&] {
        int value = *pointer;

        escape(value);
        escape(pointer);
    });
}

double custom_shared_dereference() {
    SharedPtr<int> pointer(new int(42));

    return measure([&] {
        int value = *pointer;

        escape(value);
        escape(pointer);
    });
}

double standard_shared_dereference() {
    std::shared_ptr<int> pointer(new int(42));

    return measure([&] {
        int value = *pointer;

        escape(value);
        escape(pointer);
    });
}



double custom_unique_move() {
    UniquePtr<int> pointer(new int(42));

    return measure([&] {
        UniquePtr<int> temporary(std::move(pointer));

        escape(temporary);

        pointer = std::move(temporary);

        escape(pointer);
    });
}

double standard_unique_move() {
    std::unique_ptr<int> pointer(new int(42));

    return measure([&] {
        std::unique_ptr<int> temporary(
            std::move(pointer)
        );

        escape(temporary);

        pointer = std::move(temporary);

        escape(pointer);
    });
}

double custom_shared_move() {
    SharedPtr<int> pointer(new int(42));

    return measure([&] {
        SharedPtr<int> temporary(
            std::move(pointer)
        );

        escape(temporary);

        pointer = std::move(temporary);

        escape(pointer);
    });
}

double standard_shared_move() {
    std::shared_ptr<int> pointer(new int(42));

    return measure([&] {
        std::shared_ptr<int> temporary(
            std::move(pointer)
        );

        escape(temporary);

        pointer = std::move(temporary);

        escape(pointer);
    });
}


double raw_copy() {
    int value = 42;
    int* pointer = &value;

    return measure([&] {
        int* copy = pointer;

        escape(copy);
    });
}

double custom_shared_copy() {
    SharedPtr<int> pointer(new int(42));

    return measure([&] {
        SharedPtr<int> copy = pointer;

        escape(copy);
    });
}

double standard_shared_copy() {
    std::shared_ptr<int> pointer(new int(42));

    return measure([&] {
        std::shared_ptr<int> copy = pointer;

        escape(copy);
    });
}

void run_benchmark() {
    std::cout << std::fixed
              << std::setprecision(3);

    std::cout << "Iterations: "
              << iterations
              << '\n';

    std::cout << "\nCreation and destruction\n";
    std::cout << "---------------------------------------\n";

    print_result("raw pointer", raw_create());
    print_result("UniquePtr", custom_unique_create());
    print_result(
        "std::unique_ptr",
        standard_unique_create()
    );
    print_result("SharedPtr", custom_shared_create());
    print_result(
        "std::shared_ptr",
        standard_shared_create()
    );

    std::cout << "\nDereference\n";
    std::cout << "---------------------------------------\n";

    print_result("raw pointer", raw_dereference());
    print_result(
        "UniquePtr",
        custom_unique_dereference()
    );
    print_result(
        "std::unique_ptr",
        standard_unique_dereference()
    );
    print_result(
        "SharedPtr",
        custom_shared_dereference()
    );
    print_result(
        "std::shared_ptr",
        standard_shared_dereference()
    );

    std::cout << "\nMove round trip\n";
    std::cout << "---------------------------------------\n";

    print_result(
        "UniquePtr",
        custom_unique_move()
    );
    print_result(
        "std::unique_ptr",
        standard_unique_move()
    );
    print_result(
        "SharedPtr",
        custom_shared_move()
    );
    print_result(
        "std::shared_ptr",
        standard_shared_move()
    );

    std::cout << "\nCopy and destruction\n";
    std::cout << "---------------------------------------\n";

    print_result("raw pointer", raw_copy());
    print_result(
        "SharedPtr",
        custom_shared_copy()
    );
    print_result(
        "std::shared_ptr",
        standard_shared_copy()
    );

}
