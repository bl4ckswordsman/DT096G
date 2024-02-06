//
// Created by amarildo on 2024-01-26.
//

#ifndef L1_PARSING_LEXER_H
#define L1_PARSING_LEXER_H

#include <string>
#include <vector>

#include "ops.h"

//GRAMMAR:
/**
 *  <MATCH>                ->  <WORD>
 *  <EXPR>                    ->  <OR>  |  <REPEAT>  |  <GROUP>  |  <ANY>  |  <COUNT>  |  <IGNORE_CASE>  |  <OUTPUT>  |  <WORD>
     <OR>                       ->  <OPERAND>  +  <OPERAND>
     <REPEAT>               ->  <OPERAND>  *
     <GROUP>                ->  (  <EXPR>  )
     <ANY>                     ->  .
     <COUNT>               ->  <OPERAND>  {  <NUMBER>  }
     <IGNORE_CASE>    ->  <EXPR>  \I
     <OUTPUT>             ->  <EXPR>  \O{  <NUMBER>  }
     <OPERAND>           ->  <CHAR>  |  <WORD>
     <WORD>                 ->  <CHAR> [<WORD>]
     <NUMBER>             ->  <DIGIT>  |  <DIGIT>  <NUMBER>
     <CHAR>                  ->  any non-special character */

using it = std::string::iterator;

class Lexer {
public:
    enum {
        OR, // +
        REPEAT, // *
        GROUP_START, // (
        GROUP_END, // )
        ANY, // .
        COUNT_START, // {
        COUNT_END, // }
        IGNORE_CASE, // \I
        OUTPUT, // \O{
        NUMBER, // <DIGIT>  |  <DIGIT>  <NUMBER>
        CHAR, // any non-special character
        WORD, // <CHAR> [<WORD>]
        DIGIT, // 0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9
        INVALID, // invalid input 13
        END
    };

    int type = 13; //invalid input
    char check(it &first, it last);

    static int get_next_token(it &first, const it &last);
};


#endif //L1_PARSING_LEXER_H
