//
// Created by wangrl on 2021/3/19.
//

#include <cstdio>

/**
 * 参考链接
 *
 * https://en.cppreference.com/w/cpp/language/copy_constructor
 *
 * A copy constructor of class T is a non-template constructor whose first parameter is
 * T&, const T&, and either there are no other parameters, or the rest of the parameters
 * all have default values.
 *
 * 语法
 *
 * class_name(const class_name&);
 * class_name(const class_name&) = default;
 * class_name(const class_name&) = delete;
 *
 * 使用场景
 *
 * The copy constructor is called whenever an object is initialized from another object of the
 * same type, which includes
 *
 * 1. Initialization: T a = b; or T a(b), where b is of type T;
 * 2. function argument passing: f(a); where a is of type T and f is void f(T t);
 * 3. function return: return a; inside a function such as T f(), where a is of type T, which
 *    no move constructor.
 *
 * 自动生成构造器
 *
 * If no user-defined copy constructor are provided for a class type, the compiler will always
 * declare a copy constructor as a non-explicit inline public member of its class.
 */

struct A {
    int n;

    A(int n = 1) : n(n) {
        printf("A normal ctor\n");
    }

    A(const A& a) : n(a.n) {
        printf("User defined copy ctor\n");
    }
};

struct B : A {
    // implicit default ctor B::B()
    // implicit copy ctor B::B(const B&)
};

struct C : B {
    C() : B() {
        printf("C ctor\n");
    }

private:
    C(const C&);        // no-copyable, C++98 style
};


int main(int argc, char** argv) {
    A a1(7);
    A a2(a1);       // calls the copy ctor

    B b;
    B b2 = b;

    A a3 = b;       // conversion to A& and copy ctor
    volatile A va(10);

    // 先调用父类构造器
    // 然后调用本身构造器
    C c;

    return 0;
}
