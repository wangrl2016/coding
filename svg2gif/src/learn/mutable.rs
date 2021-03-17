// Mutable data can be mutably borrowed using &mut T.

#[allow(dead_code)]
struct Book {
    // &'static str is a reference to a string allocated in read only memory.
    author: &'static str,
    title: &'static str,
    year: u32,
}

// This function takes a reference to a book.
fn _borrow_book(book: &Book) {
    println!("I immutably borrowed {} - {} edition", book.title, book.year);
}

// This function takes a references to a mutable book and changes year to 2014
fn _new_edition(book: &mut Book) {
    book.year = 2014;
    println!("I mutably borrowed {} - {} edition", book.title, book.year);
}

#[test]
fn test_mutable() {
    // Create an immutable Book named immutable_book.
    let immutable_book = Book {
        // string literals have type &'static str
        author: "Douglas",
        title: "Escher",
        year: 1979,
    };

    let mut mutable_book = immutable_book;

    // Immutably borrow an mutable object.
    _borrow_book(&mutable_book);

    // Borrow a mutable object as mutable.
    _new_edition(&mut mutable_book);
}