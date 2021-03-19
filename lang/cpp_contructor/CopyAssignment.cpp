//
// Created by wangrl on 2021/3/19.
//

#include <string>
#include <iostream>
#include <memory>

/**
 * 参考链接
 *
 * https://en.cppreference.com/w/cpp/language/copy_assignment
 *
 * A copy assignment operator of class T is a non-template non-static member function
 * with the name operator= that takes exactly one parameter of T, T&, const T&.
 *
 * 定义
 *
 * class_name& class_name::operator=(class_name)
 * class_name& class_name::operator=(const class_name&)
 * class_name& class_name::operator=(const class_name&) = default;
 * class_name& class_name::operator=(const class_name&) = delete;
 *
 * If no user-defined copy assignment operators are provided for a class type, the
 * compiler will always declare one as an inline public member of class.
 *
 * 和复制构造器的一个很大不同的区别就是没有在创建变量的时候赋值。
 *
 * Copy / Copy assigment
 * T a = b; 和 T a; a = b;的区别。
 */

struct A {
    int n;
    std::string s1;

    A() = default;

    A(A const&) = default;

    // user-defined copy assignment (copy-and-swap idiom)
    A& operator=(A other) {
        std::cout << "Copy assigment of A\n";
        std::swap(n, other.n);
        std::swap(s1, other.s1);
        return *this;
    }
};

struct B : A {
    std::string s2;
    // implicitly-defined copy assignment
};

struct C {
    std::unique_ptr<int[]> data;
    std::size_t size;

    // User-defined copy assigment (non copy-and-swap idiom)
    // note: copy-and-swap would always reallocate resources
    C& operator=(const C& other) {
        if (this != &other) { // not a self-assignment
            if (size != other.size) {   // resource cannot be reused
                data.reset(new int[other.size]);
                size = other.size;
            }
            std::copy(&other.data[0], &other.data[0] + size, &data[0]);
        }
        return *this;
    }
};

int main(int argc, char** argv) {
    A a1, a2;
    std::cout << "a1 = a2 calls ";
    a1 = a2;    // user-defined copy assignment

    B b1, b2;
    b2.s1 = "foo";
    b2.s2 = "bar";

    std::cout << "b1 = b2 calls ";
    b1 = b2;    // implicitly-defined copy assignment
    std::cout << "b1.s1 = " << b1.s1 << "; b1.s2 = " << b1.s2 << '\n';

    return 0;
}
