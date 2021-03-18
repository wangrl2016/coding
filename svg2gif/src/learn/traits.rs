// A trait is a collection of methods defined for an unknown type.

trait Animal {
    // Static method signature; Self refers to the implementor type.
    fn new(name: &'static str) -> Self;

    // Instance method signature: these will return a string.
    fn name(&self) -> &'static str;
    fn noise(&self) -> &'static str;

    // Traits can provide default method definition.
    fn talk(&self) {
        println!("{} sya {}", self.name(), self.noise());
    }
}

struct Sheep { naked: bool, name: &'static str }

impl Sheep {
    fn is_naked(&self) -> bool {
        self.naked
    }

    fn shear(&mut self) {
        if self.is_naked() {
            // Implementor methods can use the implementor's trait methods.
            println!("{} is already naked...", self.name())
        } else {
            println!("{} get a haircut!", self.name);
            self.naked = true;
        }
    }
}

impl Animal for Sheep {
    fn new(name: &'static str) -> Self {
        Sheep { name, naked: false }
    }

    fn name(&self) -> &'static str {
        self.name
    }

    fn noise(&self) -> &'static str {
        if self.is_naked() {
            "bah?"
        } else {
            "bah!"
        }
    }

    // Default trait methods can be overridden.
    fn talk(&self) {
        // For example, we can some quiet contemplation.
        println!("{} pauses briefly... {}", self.name, self.noise());
    }
}

#[test]
fn test_traits() {
    // Type annotation is necessary in this case.
    let mut dolly: Sheep = Animal::new("Dolly");

    dolly.talk();
    dolly.shear();
    dolly.talk();
}