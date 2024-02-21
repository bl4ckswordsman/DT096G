//
// Created by amarildo on 2024-01-30.
//
#include "ops.h"

#include <iostream>

#include "Lexer.h"


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

/*bool char_op::eval(it first, it last) {
    if(first != last && ((ignore_case && tolower(*first) == tolower(character)) || *first == character)) {
        ++first;
        return true;
    }
    return false;
}*/

bool char_op::eval(it first, it last) {
    for (int i = 0; i < count; ++i) {
        if (first == last || (!ignore_case && *first != character) || (ignore_case && tolower(*first) != tolower(character))) {
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

/*bool text_op::eval(it first, it last) {
    for (int i = 0; i < count; ++i) {
        if (!children[0]->eval(first, last)) {
            return false;
        }
    }
    return true;
}*/


bool expr_op::eval(it first, it last) {
    for (auto op: children) {
        if (op->eval(first, last)) {
            return true;
        }
    }
    return false;
}

bool match_op::eval(it first, it last){
    if(first == last) {
        return false;
    }
    auto result = children[0]->eval(first, last);
    if(!result){
        return eval(first + 1, last);
    }
    return true;
}

bool group_op::eval(it first, it last) {
    for (auto op: children) {
        if (!op->eval(first, last)) {
            return false;
        }
    }
    return true;
}

bool or_op::eval(it first, const it last) {
    auto first_copy = first;
    bool lhs = children[0]->eval(first, last);
    if (lhs) {
        last_evaluated_child = 0;
        first = first_copy; // Reset the iterator to the origin// If the first child operation was not successful, evaluate the second child operational position
        return true;
    }
    const bool rhs = children[1]->eval(first, last); // If the first child operation was not successful, evaluate the second child operation
    if (rhs) {
        last_evaluated_child = 1;
    }
    return rhs;
}


/*bool any_op::eval(it first, it last) {
    if (first == last) {
        return false;
    }
    //++first;
    if (first != last) {
        return true;
    }
    return false;
}*/

/*bool any_op::eval(it first, it last) {
    if (std::distance(first, last) < count) {
        return false; // Not enough characters left in the input string
    }
    char expected_char = *first;
    for (int i = 0; i < count; ++i) {
        if (*first != expected_char) {
            return false; // Found a character that doesn't match the expected character
        }
        ++first;
    }
    return true; // All characters matched the expected character
}*/

bool any_op::eval(it first, it last) {
    if (std::distance(first, last) < count) {
        return false; // Not enough characters left in the input string
    }
    std::advance(first, count); // Move the iterator forward by 'count' positions
    return true; // If we have 'count' characters, we consider it a match
}

bool repeat_op::eval(it first, it last) {
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

bool subexpr_op::eval(it first, it last) {
    if (children.empty()) return false;
    return children[0]->eval(first, last);
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
