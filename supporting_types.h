#ifndef SUPPORTING_TYPES_H
#define SUPPORTING_TYPES_H

#include <iostream>

class unmovable {
public:
    unmovable() = default;
    unmovable(const unmovable&) = default;
    unmovable& operator=(const unmovable&) = default;
    unmovable(unmovable&&) = delete;
    unmovable& operator=(unmovable&&) = delete;
};

class uncopyable {
public:
    uncopyable() = default;
    uncopyable(const uncopyable&) = delete;
    uncopyable& operator=(const uncopyable&) = delete;
    uncopyable(uncopyable&&) = default;
    uncopyable& operator=(uncopyable&&) = default;
};
#endif
