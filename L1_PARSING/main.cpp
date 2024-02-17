#include <iostream>
#include <fstream>

#include "parser.h"

// ANSI color codes
std::string RED("\033[0;31m");
std::string GREEN("\033[1;32m");
std::string RESET("\033[0m");



std::string build_tree_string(op* node, std::string& input) {
    std::string result;
    if (auto* ignore_case_node = dynamic_cast<ignore_case_op*>(node)) {
        return result; // Skip the printing of 'I'
    }
    if (auto* any_node = dynamic_cast<any_op*>(node)) {
        result += any_node->character;         // Append the character for the any_op node
        for (auto* child : node->children) {
            result += build_tree_string(child, input);
        }
    } else if (auto* charNode = dynamic_cast<char_op*>(node)) {
        // Handle char_op instances, appending their character to the result string.
        result += charNode->character;
    } else if (auto* orNode = dynamic_cast<or_op*>(node)) {
        // Handle or_op cases by choosing the appropriate child based on last_evaluated_child.
        if (orNode->last_evaluated_child == 0) {
            result += build_tree_string(orNode->children[0], input);
        } else if (orNode->last_evaluated_child == 1) {
            result += build_tree_string(orNode->children[1], input);
        } else {
            result += build_tree_string(orNode->children[0], input) + " + " + build_tree_string(orNode->children[1], input);
        }
    } else {
        // Recursively process children for all other node types, ensuring comprehensive coverage of the parse tree.
        for (auto* child : node->children) {
            result += build_tree_string(child, input);
        }
    }
    return result;
}






void print_tree_types(op* node, int depth = 0) {
    if (node == nullptr) {
        return;
    }

    // Print the depth
    for (int i = 0; i < depth; ++i) {
        std::cout << " | ";
    }

    if (auto* any_node = dynamic_cast<any_op*>(node)) {
        std::cout << "any_op" << std::endl;
    } else if (auto* ignore_case_node = dynamic_cast<ignore_case_op*>(node)) {
        std::cout << "ignore_case_char_op" << std::endl;
    } else if (auto* char_node = dynamic_cast<char_op*>(node)) {
        std::cout << "char_op: " << char_node->character << " " << (!char_node->ignore_case ? "ignore_case=false" : "ignore case") << (char_node->ignore_case ? "ignore_case=true" : "") << std::endl;
    } else if (auto* or_node = dynamic_cast<or_op*>(node)) {
        std::cout << "or_op" << std::endl;
    } else if (auto* text_node = dynamic_cast<text_op*>(node)) {
        std::cout << "text_op" << std::endl;
    } else if (auto* expr_node = dynamic_cast<expr_op*>(node)) {
        std::cout << "expr_op" << std::endl;
    } else if (auto* match_node = dynamic_cast<match_op*>(node)) {
        std::cout << "match_op" << std::endl;
    } else if (auto* group_node = dynamic_cast<group_op*>(node)) {
        std::cout << "group_op" << std::endl;
    } else if (auto* repeat_node = dynamic_cast<repeat_op*>(node)) {
        std::cout << "repeat_op" << std::endl;
    }

    for (auto child : node->children) {
        print_tree_types(child, depth + 1);
    }
}

void print_colored(const std::string& input, const std::string& pattern) {
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


int main(int argc, char* argv[]) {
    std::string program = "hate+love"; // argv[1];
    std::string input = "Waterloo I was defeated, you won the war Waterloo promise to love you for ever more Waterloo couldn't escape if I wanted to Waterloo knowing my fate is to be with you Waterloo finally facing my Waterloo";
    auto first = program.begin();
    auto last = program.end();
    if (match_op* tree = parse_match(first, last)) {
        if (tree) {
            auto match = tree->eval(input.begin(), input.end());
            std::cout << "Match result: " << (match ? "Yes" : "No") << '\n';    // TODO: Remove output
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
