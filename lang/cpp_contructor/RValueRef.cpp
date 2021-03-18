//
// Created by wangrl on 2021/3/18.
//

/**
 * 右值引用需要解决的两个问题
 *
 * 1. Implementing move semantics
 * 2. Perfect forwarding
 *
 * An lvalue is an expression e that may appear on the left or on the right hand side
 * of an assigment, whereas an rvalue is an expression that can only appear on the
 * right hand side of an assigment.
 *
 * int a = 42;
 * int b = 43;
 *
 * // a and b are both 1-values
 * a = b;       // ok
 * b = a;       // ok
 * a = a * b;   // ok
 *
 * // a * b is an rvalue
 * int c = a * b;       // ok, rvalue on right hand side of assignment
 * a * b = 42;          // error, rvalue on left hand side of assignment
 *
 * A lvalue is an expression that refers to a memory location and allows
 * us to take address of that memory location via the & operator.
 *
 * An rvalue is an expression that is not an lvalue.
 */

int main(int argc, char** argv) {
    return 0;
}