// The ref keyword can be used to take references to the fields of a struct/tuple.
#[derive(Clone, Copy)]
#[allow(dead_code)]
struct Point { x: i32, y: i32 }

#[test]
fn test_ref_pattern() {
    let c = 'Q';

    // A ref borrow on the left side of an assigment is equivalent to
    // an & borrow on the right side.
    let ref ref_c1 = c;
    let ref_c2 = &c;

    println!("ref_c1 equals ref_c2: {}", *ref_c1 == *ref_c2);
}