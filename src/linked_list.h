#pragma once

#include <cstdint>

template <typename T>
class LinkedList {
public:
    constexpr LinkedList(const T& t, const LinkedList* const next = nullptr)
            : t(t)
            , n(next) {
    }

    constexpr uint32_t size() const {
        return 1 + (next() ? next()->size() : 0);
    }

    constexpr T contents() const {
        return t;
    }

    constexpr const LinkedList* const next() const {
        return n;
    }

    constexpr const LinkedList* const operator[](uint32_t i) const {
        return i == 0 ? this : next()->operator[](i - 1);
    }

private:
    T t;
    const LinkedList* const n;
};
