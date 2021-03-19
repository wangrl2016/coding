//
// Created by wangrl on 2021/3/19.
//

#include <string>
#include <iostream>
#include <utility>
#include <iomanip>

/**
 * 参考链接
 *
 * https://en.cppreference.com/w/cpp/language/move_constructor
 *
 * 定义
 *
 * A move constructor of class T is a non-template constructor whose first parameter
 * is T&&, const T&&, and either there are no other parameters, or the rest of the
 * parameters all have default values.
 *
 * class_name(class_name &&);
 * class_name(class_name &&) = default;
 * class_name(class_name &&) = delete;
 *
 * 用法
 *
 * Initialization: T a = std::move(b); or T a(std::move(b));, where b is of type T;
 * function argument passing: f(std::move(a)); where a is of type of T and f is void f(T t);
 * function return: return a; inside a function such as T f(), where a is of type T which
 *                  has a move constructor.
 *
 * 定义规则
 *
 * If no user-defined move constructors are provided fro a class type, and all of the
 * following is true:
 * 1. there are no user-declared copy constructors;
 * 2. there are no user-declared copy assignment operators;
 * 3. there are no user-declared move assignment operators;
 * 4. there is no user-declared destructor.
 * then the compiler will declare a move constructor as a non-explict inline public
 * member of its class with the signature T::T(T&&).
 */

struct A {
    std::string s;
    int k;

    A() : s("test"), k(-1) {}

    A(const A& other) : s(other.s), k(other.k) {
        std::cout << "Copy ctor, move failed\n";
    }

    A(A&& other) noexcept:
            s(std::move(other.s)),      // explicit move of a member of class type
            k(std::exchange(other.k, 0)) // explicit move of a member of non-class type
    {
        std::cout << "A move ctor\n";
    }
};

A fun(A a) {
    return a;
}

struct B : A {
    std::string s2;
    int n;
    // implicit move constructor B::B(&&)
    // calls A's move constructor
    // calls s2's move constructor
    // and makes a bitwise copy of n
};

struct C : B {
    ~C() {}   // destructor prevents implicit move constructor C::(C&&)
};

struct D : B {
    D() {}

    ~D() {};    // destructor would prevent implicit move constructor D::(D&&)

    D(D&&) = default;   // forces a move constructor anyway
};

int main(int argc, char** argv) {
    std::cout << "Trying to move A\n";
    A a1 = fun(A());
    std::cout << "Before move, a1.s = " << std::quoted(a1.s) << " a1.k = " << a1.k << "\n";
    A a2 = std::move(a1);   // move-constructs from xvalue
    std::cout << "After move, a1.s = " << std::quoted(a1.s) << " a1.k = " << a1.k << "\n";

    std::cout << "Trying to move B\n";
    B b1;
    std::cout << "Before move, b1.s = " << std::quoted(b1.s) << "\n";
    B b2 = std::move(b1);   // calls implicit move constructor
    std::cout << "After move, b1.s = " << std::quoted(b1.s) << "\n";

    std::cout << "Trying to move C\n";
    C c1;
    C c2 = std::move(c1);   // calls copy constructor

    std::cout << "Trying to move D\n";
    D d1;
    D d2 = std::move(d1);

    return 0;
}
