//
// Created by amarildo on 2024-01-30.
//
#include "ops.h"

#include <iostream>

#include "Lexer.h"

// Static variable to keep track of the output group number
int output_group = 0;

// Static container to keep track of the captured groups
std::vector<std::string> captured_groups;

bool op::eval(it first, it last) {
    for (auto op: children) {
        if (op->eval(first, last)) {
            return true;
        }
    }
    return false;
}

void op::add(op *child) {
    if (child) {
        children.push_back(child);
    }
}

void op::set_ignore_case(bool ignore_case) {
    this->ignore_case = ignore_case;
}

bool char_op::eval(it first, it last) {
    for (int i = 0; i < count; ++i) {
        if (first == last || (!ignore_case && *first != character) || (
                ignore_case && tolower(*first) != tolower(character))) {
            return false;
        }
        ++first;
    }
    return true;
}

bool text_op::eval(it first, it last) {
    auto result = children[0]->eval(first, last);
    if (children.size() > 1) {
        return result && children[1]->eval(first + 1, last);
    }
    return result;
}

bool expr_op::eval(it first, it last) {
    bool any_child_evaluated = false;
    for (auto op: children) {
        if (op->eval(first, last)) {
            any_child_evaluated = true;
        }
    }
    return any_child_evaluated;
}

bool match_op::eval(it first, it last) {
    if (first == last) {
        return false;
    }
    auto result = children[0]->eval(first, last);
    if (!result) {
        return eval(first + 1, last);
    }
    return true;
}

bool group_op::eval(it first, it last) {
    auto start = first;
    for (auto op: children) {
        op->capture(first, last);
        if (!op->eval(first, last)) {
            return false;
        }
    }
    if (start != first) {
        captured_groups.push_back(std::string(start, first));
    }
    return true;
}

void group_op::capture(it &first, it &last) {
    auto start = first;
    for (auto op: children) {
        op->eval(first, last);
        op->capture(first, last); // Call capture on the child nodes
    }
    if (start != first) {
        captured_groups.push_back(std::string(start, first));
    }
}

bool or_op::eval(it first, const it last) {
    auto first_copy = first;
    bool lhs = children[0]->eval(first, last);
    if (lhs) {
        last_evaluated_child = 0;
        first = first_copy;
        // Reset the iterator to the origin// If the first child operation was not successful, evaluate the second child operational position
        return true;
    }
    const bool rhs = children[1]->eval(first, last);
    // If the first child operation was not successful, evaluate the second child operation
    if (rhs) {
        last_evaluated_child = 1;
    }
    return rhs;
}

bool any_op::eval(it first, it last) {
    if (std::distance(first, last) < count) {
        return false; // Not enough characters left in the input string
    }
    std::advance(first, count); // Move the iterator forward by 'count' positions
    return true; // If we have 'count' characters, we consider it a match
}

bool star_op::eval(it first, it last) {
    if (first == last) {
        return false;
    }
    char operand = *first;
    while (first != last && *first == operand) {
        ++first;
    }
    return true;
}

bool ignore_case_op::eval(it first, it last) {
    for (auto &c: children) {
        ignore_case_for_all(c);
    }
    /*it temp = first;*/ //TODO: Remove this line
    if (children[0]->eval(first, last)) {
        return true;
    }
    return false;
}

void ignore_case_op::ignore_case_for_all(op *node) {
    node->set_ignore_case(true);
    for (auto &c: node->children) {
        ignore_case_for_all(c);
    }
}

bool count_op::eval(it first, it last) {
    if (children.empty()) {
        return false; // or handle the error in some other way
    }
    for (int i = 0; i < count; ++i) {
        if (!children[0]->eval(first, last)) {
            return false;
        }
    }
    return true;
}

bool output_op::eval(it first, it last) {
    if (group_index < captured_groups.size()) {
        /*std::cout << "  ---Captured group: "<< captured_groups[group_index] << std::endl;  //TODO: Debug print*/
        return true;
    }
    return false;
}
