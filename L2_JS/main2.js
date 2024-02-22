
Array.prototype.iter = function() {
    let index = 0;
    let array = this;
    return {
        hasNext: function () {
            return index < array.length;
        },
        next: function () {
            if (this.hasNext()) {
                return array[index++];
            }
            console.log("No more elements");
            return null;
        },
    }
}

array = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];

it = array.iter();

while (it.hasNext()) {
    console.log(it.next());
}
