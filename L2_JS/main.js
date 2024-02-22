
class array_it {
    index;
    array;
    constructor(arr) {
        this.index = 0;
        this.array = arr;
    }

    hasNext() {
        return this.index < this.array.length;
        //return this.index < this.array.length;
    }

    next() {
        if (this.hasNext()) {
            return this.array[this.index++];
            //return this.array[this.index++];
        }
        console.log("No more elements");
        return null;
        //return this.array[this.index++];
    }
}

array_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
it = new array_it(array_list);

while (it.hasNext()) {
    console.log(it.next());
}
