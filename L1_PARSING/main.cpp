#include <iostream>
#include <fstream>

#include "parser.h"

// ANSI color codes
std::string RED("\033[0;31m");
std::string GREEN("\033[1;32m");
std::string BLUE("\033[1;34m");
std::string RESET("\033[0m");


std::string build_tree_string(op *node, std::string &input) {
    std::string result;
    std::string type = node->get_type();

    if (type == "any_op") {
        auto *any_node = static_cast<any_op *>(node);
        result += any_node->character;
    } else if (type == "char_op") {
        auto *charNode = static_cast<char_op *>(node);
        result += charNode->character;
    } else if (type == "or_op") {
        auto *orNode = static_cast<or_op *>(node);
        if (orNode->last_evaluated_child == 0) {
            result += build_tree_string(orNode->children[0], input);
        } else if (orNode->last_evaluated_child == 1) {
            result += build_tree_string(orNode->children[1], input);
        } else {
            result += build_tree_string(orNode->children[0], input) + " + " + build_tree_string(
                orNode->children[1], input);
        }
    } else if (type == "ignore_case_op") {
        for (auto *child: node->children) {
            result += build_tree_string(child, input);
        }
    } else {
        for (auto *child: node->children) {
            result += build_tree_string(child, input);
        }
    }
    return result;
}


void print_tree_types(op *node, int depth = 0, const std::string &prefix = "") {
    if (!node) return;

    std::string type = node->get_type();
    std::cout << prefix << "+--" << type;
    if (type == "char_op") {
        char_op *charNode = static_cast<char_op *>(node);
        std::cout << ": " << charNode->character << BLUE << " ignore_case=" << std::boolalpha << charNode->ignore_case
                << RESET;
    }
    std::cout << std::endl;

    for (size_t i = 0; i < node->children.size(); ++i) {
        print_tree_types(node->children[i], depth + 1, prefix + (i < node->children.size() - 1 ? "|   " : "    "));
    }
}

void print_colored(const std::string &input, const std::string &pattern) {
    // Extract the fixed part of the pattern and count the dots
    size_t fixedPartLen = pattern.find('.');
    if (fixedPartLen == std::string::npos) {
        fixedPartLen = pattern.length();
    }
    size_t dotCount = pattern.length() - fixedPartLen; // Assumes dots are at the end

    for (size_t i = 0; i < input.length();) {
        if (i + fixedPartLen + dotCount <= input.length()) {
            std::string substring = input.substr(i, fixedPartLen + dotCount);
            // Check if the fixed part matches and if the rest of the substring fits the pattern criteria (dots)
            if (input.substr(i, fixedPartLen) == pattern.substr(0, fixedPartLen)) {
                std::cout << GREEN << substring;
                i += fixedPartLen + dotCount; // Move past the matched pattern
                continue; // Skip the rest of the loop to avoid printing the same characters in red
            }
        }
        // If no match, print the character in red and move to the next character
        std::cout << RED << input[i];
        i++;
    }
    std::cout << RESET; // Reset the color at the end
    std::cout << std::endl;
}


int main(int argc, char *argv[]) {
    std::string program = "facing my Waterlo.."; // argv[1];
    std::string input =
            "Waterloo I was defeated, you won the war Waterloo promise to love you for ever more Waterloo couldn't escape if I wanted to Waterloo knowing my fate is to be with you Waterloo finally facing my Waterloo";
    auto first = program.begin();
    auto last = program.end();
    if (match_op *tree = parse_match(first, last)) {
        if (tree) {
            auto match = tree->eval(input.begin(), input.end());
            std::cout << "Match result: " << (match ? "Yes" : "No") << '\n'; // TODO: Remove output
            std::cout << build_tree_string(tree, input) << '\n';
            print_colored(input, build_tree_string(tree, input));
            print_tree_types(tree);
            return EXIT_SUCCESS;
        } else {
            std::cout << "Parsing failed.\n";
        }
    }
    return EXIT_FAILURE;
}
