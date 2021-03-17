// Most of the time, we'd like to access data without taking ownership over it.
// To accomplish this, Rust uses a borrowing mechanism. Instead of passing objects
// by value(T), objects can be passed by reference(&T).

// This function takes ownership of a box and destroys it.
fn eat_box_i32(boxed_i32: Box<i32>) {
    println!("Destroying box that contains {}", boxed_i32);
}

// This function borrows an i32
fn borrow_i32(borrowed_i32: &i32) {
    println!("This int is: {}", borrowed_i32);
}

#[test]
fn test_borrowing() {
    // Create a boxed i32, and a stacked i32.
    let boxed_i32 = Box::new(5_i32);
    let stack_i32 = 6_i32;

    // Borrow the contents of the box.
    // Ownership is not taken,
    // So the contents can be borrowed again.
    borrow_i32(&boxed_i32);
    borrow_i32(&stack_i32);

    {
        // Take a reference to the data contained inside the box.
        let _ref_to_i32: &i32 = &boxed_i32;

        // Can't destroy boxed_i32 while the inner value is borrowed later in scope.
        // eat_box_i32(boxed_i32);

        // Attempt to borrow _ref_to_i32 after inner value is destroyed.
        borrow_i32(_ref_to_i32);
        // _ref_to_i32 goes out of scope and is no longer borrowed.
    }

    // boxed_i32 can now given up ownership to eat_box and be destroyed.
    eat_box_i32(boxed_i32);
}