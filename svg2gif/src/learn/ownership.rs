// This function takes ownership of the heap allocated memory.
fn destroy_box(c: Box<i32>) {
    println!("Destroying a box that contains {}", c);
}

#[test]
fn test_ownership() {

    // Stack allocated integer.
    let x = 5u32;

    // *Copy* x into y - no resources are moved.
    let _y = x;

    // a is a pointer to a heap allocated integer.
    let a = Box::new(5i32);
    println!("A contains: {}", a);

    // Move a into b.
    let b = a;
    // The pointer address of a is copied (not the data) into b.
    // Both are now pointers to the same heap allocated data, but
    // b now own it.

    // Error! a can no longer access the data, because it no longer owns the heap memory.
    // println!("a contains: {}", a);

    // This function take ownership of the heap allocated memory from b.
    destroy_box(b);

    // Since the heap memory has been free at this point, this action would
    // result in dereferencing freed memory, but it's forbidden by the compiler
    // Error! Same reason as the previous error.
    // println!("b contains: {}", b);
}