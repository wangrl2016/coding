// 资源获取即初始化(Resource Acquisition Is Initialization)
// RAII要求，资源的有效期与持有志远的对象生命周期严格绑定，即由对象的构造函数完成资源的分配，
// 同时由析构函数完成资源的释放。
// Rust enforces RAII, so whenever an object goes out of scope, its destructor is called and
// its owned resources are freed.
fn _create_box() {
    // Allocate an integer on the heap
    let _box1 = Box::new(3i32);

    // _box1 is destroyed here, and memory gets freed.
}

// The notion of a destructor in Rust is provided through the Drop trait.

struct ToDrop;

impl Drop for ToDrop {
    fn drop(&mut self) {
        println!("ToDrop is being dropped");
    }
}

#[test]
fn test_raii() {
    // Allocate an integer on the heap.
    let _box2 = Box::new(5i32);

    // A nested scope.
    {
        // Allocate an integer on the heap.
        let _box3 = Box::new(4i32);

        // _box3 is destroy here, and memory gets freed.
    }

    // Creating lots of boxes just for fun.
    // There's no need to manually free memory!
    for _ in 0u32..100 {
        _create_box();
    }

    // _box2 is destroy here, and memory gets freed.
}

#[test]
fn test_destructor() {
    let _x = ToDrop;
    println!("Made a ToDrop!");
}


