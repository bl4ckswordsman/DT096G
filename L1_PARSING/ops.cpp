//
// Created by amarildo on 2024-01-30.
//
#include "ops.h"

#include "Lexer.h"


bool op::eval(it first, it last) {
    for (auto op: children) {
        if (op->eval(first, last)) {
            return true;
        }
    }
    return false;
}

bool char_op::eval(it first, it last) {
    return *first == character;
}

bool text_op::eval(it first, it last) {
    auto result = children[0]->eval(first, last);
    if (children.size() > 1) {
        return result && children[1]->eval(first + 1, last);
    }
    return result;
}


bool expr_op::eval(it first, it last) {
    for (auto op: children) {
        if (op->eval(first, last)) {
            return true;
        }
    }
    return false;
}

/*bool match_op::eval(it first, it last) {
    for (auto op: children) {
        if (!op->eval(first, last)) {
            return false;
        }
    }
    return true;
}*/

bool match_op::eval(it first, it last) {
    for (auto start = first; start != last; ++start) {
        auto temp = start; // Temporary iterator to track the current position
        for (auto op: children) {
            if (!op->eval(temp, last)) {
                break; // If not found, check the next character in the text for match
            }
            if (op == children.back()) {
                return true; // If the last character is found, return true
            }
        }
    }
    return false;
}

bool group_op::eval(it first, it last) {
    for (auto op: children) {
        if (!op->eval(first, last)) {
            return false;
        }
    }
    return true;
}

bool or_op::eval(it first, it last) {
    auto first_copy = first;
    bool lhs = children[0]->eval(first, last);
    if (lhs) {
        first = first_copy;
        return true;
    }
    return children[1]->eval(first, last);
}

bool any_op::eval(it first, it last) {
    if (first == last) {
        return false;
    }
    ++first;
    return true;
}
