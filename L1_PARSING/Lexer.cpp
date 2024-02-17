//
// Created by amarildo on 2024-01-26.
//

#include <fstream>
#include <iostream>
#include <map>
#include "Lexer.h"

int Lexer::check(it &first, it last) {
    if (first == last) {
        type = END;
        return type;
    }
    switch (*first) {
        case '+':
            type = OR;
            break;
        case '*':
            type = REPEAT;
            break;
        case '(':
            type = GROUP_START;
            break;
        case ')':
            type = GROUP_END;
            break;
        case '.':
            type = ANY;
            break;
        case '{':
            type = COUNT_START;
            break;
        case '}':
            type = COUNT_END;
            break;
        case '\\': {
            auto next = std::next(first, 1);
            if (next != last) {
                if (*next == 'I') {
                    type = IGNORE_CASE;
                    //first = std::next(first, 2); // Skip the 'I' character and the character after it
                } else if (*next == 'O') {
                    type = OUTPUT;
                    ++first;
                }
            } else {
                std::cerr << "Invalid input: " << *first << std::endl;
            }
            break;
        }
        default:
            if (isdigit(*first)) {
                type = DIGIT;
            }
            if (isalnum(*first) || isspace(*first)) {
                type = CHAR;
            } else {
                type = INVALID;
            }
            break;
    }
    return type;
}

int Lexer::get_next_token(it &first, const it &last) {
    Lexer lexer;
    if (first != last) {
        lexer.check(first, last);
        return lexer.type;
    }
    return END;
}
