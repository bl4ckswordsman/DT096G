class ArrayIterator {
    constructor(arr) {
        this.index = 0; // Initialize index
        this.array = arr; // Initialize array
    }

    hasNext() {
        return this.index < this.array.length; // Check if more elements exist
    }// "this" refers to the context of func call. determined by where func is invoked, not where it's defined

    next() {
        if (this.hasNext()) { // dot is used for properties
            return this.array[this.index++]; // Return next element if exists
        }
        console.log("No more elements"); // Log message if no more elements
        return null;
    }
}

const arrayList = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]; // Define array
const iterator = new ArrayIterator(arrayList); // Create iterator

while (iterator.hasNext()) { // Iterate over array
    console.log(iterator.next());
}