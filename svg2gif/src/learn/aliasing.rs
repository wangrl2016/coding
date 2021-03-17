#[allow(dead_code)]
struct Point { x: i32, y: i32, z: i32 }

#[test]
fn test_aliasing() {
    let mut point = Point { x: 0, y: 0, z: 0 };

    let borrowed_point = &point;
    let another_borrowed = &point;

    // Data can be accessed via the references and the original owner.
    println!("Point has coordinates: ({}, {}, {})",
             borrowed_point.x, another_borrowed.y, point.z);

    // Error! Can't borrow point as mutable because it's currently
    // borrowed as immutable.
    // let mutable_borrow = &mut point;

    // The borrowed values are used again here.
    println!("Point has coordinates: ({}, {}, {})",
             borrowed_point.x, another_borrowed.y, point.z);

    // The immutable references are no longer used for the rest of the code so
    // it is possible to re-borrow with a mutable reference.
    let mutable_borrow = &mut point;

    // Change data via mutable reference.
    mutable_borrow.x = 5;
    mutable_borrow.y = 2;
    mutable_borrow.z = 1;

    // Ok! mutable references can be passed as immutable to println!
    println!("Point has coordinate: ({}, {}, {})",
             mutable_borrow.x, mutable_borrow.y, mutable_borrow.z);

    // The mutable reference is no longer used for the rest of the code so it
    // is possible to re-borrow.
    let new_borrowed_point = &point;
    println!("Point now has coordinates: ({}, {}, {})",
             new_borrowed_point.x, new_borrowed_point.y, new_borrowed_point.z);
}