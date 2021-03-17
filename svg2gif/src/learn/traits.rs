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
            println!("{} get a ha")
        }
    }
}

impl Animal for Sheep {
    fn new(name: &'static str) -> Self {
        unimplemented!()
    }

    fn name(&self) -> &'static str {
        unimplemented!()
    }

    fn noise(&self) -> &'static str {
        unimplemented!()
    }
}