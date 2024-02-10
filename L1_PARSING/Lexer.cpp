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
            if (next != last && *next == 'I') {
                type = IGNORE_CASE;
                ++first; // Move to the next character safely
            } else if (next != last && *next == 'O') {
                type = OUTPUT;
                ++first;
            }
            break;
        }
        /*case '\\': {
            if (*(first + 1) == 'I') {
                type = IGNORE_CASE;
            }
            if (*(first + 1) == 'O' && *(first + 2) == '{') {
                type = OUTPUT;
            }
        }*/
        default:
            if (isdigit(*first)) {
                type = DIGIT;
            }
            if (isalnum(*first) || *first == ' ') {
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
