//
// Created by wangrl on 2021/4/6.
//

#pragma once

namespace sf {
    /**
     * Utility class that makes any derived class non-copyable.
     *
     * This class makes its instances non-copyable, by explicitly
     * disabling its copy constructor and its assignment operator.
     *
     * To create a non-copyable class, simply inherit from
     * sf::NonCopyable.
     *
     * Deciding whether the instances of a class can be copied
     * or not is very important design choice. You are strongly
     * encouraged to think about it before writing a class,
     * and to use sf::NonCopyable when necessary to prevent
     * many potential future errors when using it. This is also
     * a very important indication to users of your class.
     */
    class NonCopyable {
    protected:
        /**
         * Because this class has a copy constructor, the compiler
         * will not automatically generate the default constructor.
         * That's why we must define it explicitly.
         */
        NonCopyable() = default;

        /**
         * By declaring a protected destructor it's impossible to
         * call delete on a pointer of sf::NonCopyable, thus
         * preventing possible resource leaks.
         */
        ~NonCopyable() = default;

    public:
        /*
         * Disabled copy constructor.
         */
        NonCopyable(const NonCopyable&) = delete;

        /**
         * Disabled assignment operator.
         */
        NonCopyable& operator=(const NonCopyable&) = delete;
    };
}