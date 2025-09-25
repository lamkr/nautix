use crate::db::Identificable;

#[derive(Debug, Clone)]
pub struct Person {
    pub id: usize,
    pub name: String,
}

impl Person {
    pub fn new(name: String) -> Self {
        Person { id: 0, name }
    }
}

impl Identificable for Person {
    fn id(&self) -> usize {
        self.id
    }

    fn set_id(&mut self, id: usize) {
        self.id = id;
    }
}
