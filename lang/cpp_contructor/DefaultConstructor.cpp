//
// Created by wangrl on 2021/3/19.
//

#include <cstdio>

/**
 * A default constructor is a constructor is a constructor which can be called with no arguments.
 * (either defined with an empty parameter list, or with default arguments provides for every
 * parameter).
 *
 * 如果用户没有写默认构造器，那么编译器会生成默认构造器。
 *
 * If no user-declared constructors of any kind are provided for a class type (struct, class, or union),
 * the compiler will always declare a default constructor as an inline public member of its class.
 *
 *
 */

struct A {
    int x;

    A(int x = 1) : x(x) {
        printf("A con called\n");
    }
};

struct B : A {
    // B::B() is implicitly-defined, calls A::A().
};

struct C {
    A a;
    // C::C() is implicitly-defined, calls A::A().
};

struct D : A {
    D(int y) : A(y) {}
    // D::D() is not declared because another constructor exists.
};

struct E : A {
    E(int y) : A(y) {}

    E() = default;      // explicitly defaulted, calls A::A()
};

struct F {
    int& ref;       // reference member
    const int c;    // const member
    // F::F() is implicitly defined as deleted
};

int main(int argc, char** argv) {
    A a;
    B b;
    C c;

    // D d;     // compile error

    E e;

    // F f;     // compiler error
}
