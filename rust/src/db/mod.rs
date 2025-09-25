use std::collections::HashMap;

// Define um comportamento.
// Qualquer struct que implemente esse trait pode ser usado no Db.
pub trait Identificable {
    fn id(&self) -> usize;
    fn set_id(&mut self, id: usize);
}

pub struct Db<T: Identificable> {
    data: HashMap<usize, T>,
    next_id: usize,
}

impl<T: Identificable> Db<T> {
    pub fn new() -> Self {
        Db {
            data: HashMap::new(),
            next_id: 1
        }
    }

    pub fn create(&mut self, mut item: T) -> usize {
        item.set_id(self.next_id);
        self.data.insert(self.next_id, item);
        self.next_id += 1;
        self.next_id - 1
    }

    pub fn read(&self, id: usize) -> Option<&T> {
        self.data.get(&id)
    }

    pub fn read_all(&self) -> Vec<&T> {
        self.data.values().collect()
    }

    pub fn update(&mut self, item: T) -> Option<T> {
        self.data.insert(item.id(), item)
    }

    pub fn delete(&mut self, id: usize) -> Option<T> {
        self.data.remove(&id)
    }

    pub fn length(&self) -> usize {
        self.data.len()
    }
}
