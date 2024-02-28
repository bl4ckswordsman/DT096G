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
op* parse_group(it& first, it last);
text_op* parse_text(it &first, it last);
text_op* parse_any(it& first, it last);
star_op* parse_star(it& first, it last);
ignore_case_op* parse_ignore_case(it& first, it last);
op* parse_basic_opration(it& first, it last);
void parse_post_group_operations(op *group, it& first, it last);
count_op* parse_count(it& first, it last);
int parse_number(it& first, it last);
output_op* parse_output(it& first, it last);

#endif //L1_PARSING_PARSER_H
