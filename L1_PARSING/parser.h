//
// Created by amarildo on 2024-01-26.
//

#ifndef L1_PARSING_PARSER_H
#define L1_PARSING_PARSER_H
#include "ops.h"

using it = std::string::iterator;


or_op* parse_or(it first, it last);
char_op* parse_char(it& first, it last);
expr_op* parse_expr(it& first, it last);
match_op* parse_match(it& first, it last);
group_op* parse_group(it& first, it last);
text_op* parse_text(it &first, it last);
any_op* parse_any(it& first, it last);
repeat_op* parse_repeat(it& first, it last);
ignore_case_op* parse_ignore_case(it& first, it last);

#endif //L1_PARSING_PARSER_H
