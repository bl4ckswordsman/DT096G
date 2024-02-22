Array.prototype.iterator = function() {
    let index = 0; // Initialize index
    const array = this; // Initialize array

    return {
        hasNext: function () {
            return index < array.length; // Check if more elements exist
        },
        next: function () {
            if (this.hasNext()) {
                return array[index++]; // Return next element if exists
            }
            console.log("No more elements"); // Log message if no more elements
            return null;
        },
    }
}

const array = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]; // Define array
const iterator = array.iterator(); // Create iterator

while (iterator.hasNext()) { // Iterate over array
    console.log(iterator.next());
}