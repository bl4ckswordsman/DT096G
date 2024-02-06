//
// Created by amarildo on 2024-01-26.
//

#include <fstream>
#include <iostream>
#include <map>
#include "Lexer.h"

char Lexer::check(it &first, it last) {
    if (first == last) {
        type = END;
        return '$';
    }
    switch (*first) {
        case '+':
            type = OR;
            return '+';
        case '*':
            type = REPEAT;
            return '*';
        case '(':
            type = GROUP_START;
            return '(';
        case ')':
            type = GROUP_END;
            return ')';
        case '.':
            type = ANY;
            return '.';
        case '{':
            type = COUNT_START;
            return '{';
        case '}':
            type = COUNT_END;
            return '}';
        case '\\': {
            if (*(first + 1) == 'I') {
                type = IGNORE_CASE;
                return 'I';
            }
            if (*(first + 1) == 'O' && *(first + 2) == '{') {
                type = OUTPUT;
                return 'O';
            }
        }
        default:
            if (isdigit(*first)) {
                type = DIGIT;
                return *first;
            }
            if (isalpha(*first)) {
                type = CHAR;
                return *first;
            }
            type = INVALID;
            return *first;
    }
}

int Lexer::get_next_token(it &first, const it &last) {
    Lexer lexer;
    if (first != last) {
        return lexer.check(first, last);
    }
    return END;
}
