/**
 * Implementation of an iterator pattern using a regular object/class.
 */

class ArrayIterator {
    constructor(arr) {
        this.index = 0; // Initialize index
        this.array = arr; // Initialize array
    }

    hasNext() {
        return this.index < this.array.length; // Check if more elements exist
    }// "this" refers to the instance of the class

    next() {
        if (this.hasNext()) { // dot is used for properties
            return this.array[this.index++]; // "this" refers to the instance of the class
        }
        // console.log("No more elements"); // Log message if no more elements
        return null;
    }
}

// Add iterator method to Array prototype
Array.prototype.iterator = function() {
    return new ArrayIterator(this);     // the array is passed to the constructor
}

const arrayList = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]; // Define array
const iterator = arrayList.iterator(); // Create iterator

while (iterator.hasNext()) { // Iterate over array
    console.log(iterator.next());
}