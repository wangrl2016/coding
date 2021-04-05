use std::cell::RefCell;
use std::rc::Rc;
use usvg::ScreenSize;
use crate::layers::List::{Cons, Nil};

type LayerData = Rc<RefCell<tiny_skia::Pixmap>>;

// Stack of image layers.
// Instead of creating a new layer each time we need one,
// we are reusing an existing one.
pub struct Layers {
    d: Vec<LayerData>,
    // Use Rc as a shared counter.
    counter: Rc<()>,
    img_size: ScreenSize,
}

impl Layers {
    // Creates Layers
    pub fn new(img_size: ScreenSize) -> Self {
        Layers {
            d: Vec::new(),
            counter: Rc::new(()),
            img_size,
        }
    }
}

// https://doc.rust-lang.org/book/ch15-04-rc.html
// Rust has a type called Rc<T>, which is an abbreviation for Reference counting.

enum List {
    Cons(i32, Rc<List>),
    Nil,
}

#[test]
fn test_rc() {
    let a = Rc::new(Cons(5, Rc::new(Cons(10, Rc::new(Nil)))));
    assert_eq!(1, Rc::strong_count(&a));
    let b = Cons(3, Rc::clone(&a));
    let c = Cons(4, Rc::clone(&a));
    assert_eq!(3, Rc::strong_count(&a));
}

// Because RefCell<T> allows mutable borrows checked at runtime, you can mutate the value
// inside the RefCell<T> even when the RefCell<T> immutable.
// Interior Mutability Pattern
pub trait Messenger {
    fn send(&self, msg: &str);
}

pub struct LimitTracker<'a, T: Messenger> {
    messenger: &'a T,
    value: usize,
    max: usize,
}

impl<'a, T> LimitTracker<'a, T> where T: Messenger {
    pub fn new(messenger: &T, max: usize) -> LimitTracker<T> {
        LimitTracker {
            messenger,
            value: 0,
            max,
        }
    }

    pub fn set_value(&mut self, value: usize) {
        self.value = value;

        let percentage_of_max = self.value as f64 / self.max as f64;

        if percentage_of_max >= 1.0 {
            self.messenger.send("Error: You are over your quota!");
        } else if percentage_of_max >= 0.9 {
            self.messenger
                .send("Urgent warning: You've used up over 90% of your quota!");
        } else if percentage_of_max >= 0.75 {
            self.messenger
                .send("Warning: You've used up over 75% of your quota!");
        }
    }
}

#[test]
fn test_ref_cell() {
    use super::*;
    use std::cell::RefCell;
    struct MockMessenger {
        sent_messages: RefCell<Vec<String>>,
    }

    impl MockMessenger {
        fn new() -> MockMessenger {
            MockMessenger {
                sent_messages: RefCell::new(vec![]),
            }
        }
    }

    impl Messenger for MockMessenger {
        fn send(&self, message: &str) {
            self.sent_messages.borrow_mut().push(String::from(message));
        }
    }

    let mock_messenger = MockMessenger::new();
    let mut limit_tracker = LimitTracker::new(&mock_messenger, 100);
    limit_tracker.set_value(80);
    assert_eq!(mock_messenger.sent_messages.borrow().len(), 1);
}