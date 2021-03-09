//
// Created by wangrl on 2021/3/9.
//

#pragma once

/**
 * Noncopyable is the base class for objects that do not want to
 * be copied. It hides its copy-constructor and its assignment-operator.
 */
class Noncopyable {
public:
    Noncopyable() = default;

    Noncopyable(Noncopyable&&) = default;

    Noncopyable& operator=(Noncopyable&&) = default;

private:
    Noncopyable(const Noncopyable&) = delete;

    Noncopyable& operator=(const Noncopyable&) = delete;
};
