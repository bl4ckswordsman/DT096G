//
// Created by amarildo on 2024-01-26.
//

#ifndef L1_PARSING_LEXER_H
#define L1_PARSING_LEXER_H

#include <string>

#include "ops.h"

//GRAMMAR:
/**
    <MATCH>                ->  <EXPR>
    <EXPR>                    ->  <OR>  |  <STAR>  |  <GROUP>  |  <ANY>  |  <COUNT>  |  <IGNORE_CASE>  |  <OUTPUT>  |  <TEXT>
     <OR>                       ->  <TEXT>  +  <TEXT>
     <STAR>                   ->  <TEXT>  *
     <GROUP>                ->  (  <EXPR>  )
     <ANY>                    ->  .
     <COUNT>               ->  <CHAR>  {  <NUMBER>  } |  <ANY>  {  <NUMBER>  }
     <IGNORE_CASE>     ->  <TEXT>  \I
     <OUTPUT>              ->  <EXPR>  \O{  <NUMBER>  }
     <TEXT>                   ->  <CHAR> [<TEXT>]
     <NUMBER>             ->  <DIGIT>  [<NUMBER>]
     <CHAR>                  ->  any non-special character                  */

using it = std::string::iterator;

class Lexer {
public:
    enum {
        OR, // +
        STAR, // *
        GROUP_START, // (
        GROUP_END, // )
        ANY, // .
        COUNT_START, // {
        COUNT_END, // }
        IGNORE_CASE, // \I
        OUTPUT, // \O{
        NUMBER, // <DIGIT>  |  <DIGIT>  <NUMBER>
        CHAR, // any non-special character
        TEXT, // <CHAR> [<TEXT>]
        DIGIT, // 0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9
        INVALID, // invalid input 13
        END
    };

    int type = INVALID; //13; //invalid input, initialization
    int check(it &first, it last);

    static int get_next_token(it &first, const it &last);
};


#endif //L1_PARSING_LEXER_H
