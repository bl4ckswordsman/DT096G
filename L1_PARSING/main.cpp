#include <iostream>
#include <fstream>

#include "parser.h"

int main(int argc, char* argv[]) {
    std::string program = "escape"; // argv[1];
    std::string input = "Waterloo I was defeated, you won the war Waterloo promise to love you for ever more Waterloo couldn't escape if I wanted to Waterloo knowing my fate is to be with you Waterloo finally facing my Waterloo";
    auto first = program.begin();
    auto last = program.end();
    if (match_op* tree = parse_match(first, last)) {
        if (tree) {
            auto match = tree->eval(input.begin(), input.end());
            std::cout << "Match result: " << (match ? "Yes" : "No") << '\n';
        } else {
            std::cout << "Parsing failed.\n";
        }
    }
    return 0;
}
