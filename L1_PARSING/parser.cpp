//
// Created by amarildo on 2024-01-26.
//

#include "parser.h"
#include <iostream>
#include "Lexer.h"

/*<CHAR>                  ->  any non-special character */
char_op *parse_char(it &first, it last) {
    Lexer lexer;
    lexer.check(first, last);
    if (lexer.type == Lexer::END || lexer.type != Lexer::CHAR)
        return nullptr;
    return new char_op(*first);
}


/*<TEXT>    ->  <CHAR> [<TEXT>] */
text_op *parse_text(it &first, it last) {
    if (first == last ) {
        return nullptr; // Base case to stop the recursion
    }
    auto char_node = parse_char(first, last);
    if (!char_node)
        return nullptr;
    auto *result = new text_op;
    ++first;
    result->add(char_node);
    result->add(parse_text(first, last));
    return result;
}

/*<EXPR>      ->  <OR>  |  <REPEAT>  |  <GROUP>  |  <ANY>  |  <COUNT>  |  <IGNORE_CASE>  |  <OUTPUT>  |  <TEXT> */
expr_op *parse_expr(it &first, it last) {
    auto restore = first;

    auto group_op = parse_group(first, last);
    if (group_op) {
        auto expr_node = new expr_op;
        expr_node->add(group_op);
        if (first != last) ++first; // Move the iterator forward
        if (group_op->eval(first, last)) {
            expr_node->add(parse_expr(first, last));
        }
        return expr_node;
    }
    first = restore;
    any_op *anyNode = parse_any(first, last);
    if (anyNode) {
        auto expr_node = new expr_op;
        expr_node->add(anyNode);
        if (first != last) ++first; // Move the iterator forward
        if (anyNode->eval(first, last)) {
            expr_node->add(parse_expr(first, last));
        }
        return expr_node;
    }
    or_op *orNode = parse_or(first, last);
    if (orNode) {
        auto expr_node = new expr_op;
        expr_node->add(orNode);
        if (first != last) ++first; // Move the iterator forward
        if (orNode->eval(first, last)) {
            expr_node->add(parse_expr(first, last));
        }
        return expr_node;
    }
    auto text_node = parse_text(first, last);
    if (text_node) {
        auto expr_node = new expr_op;
        expr_node->add(text_node);
        expr_node->add(parse_expr(first, last));
        return expr_node;
    }
    repeat_op *repeatNode = parse_repeat(first, last);
    if (repeatNode) {
        auto expr_node = new expr_op;
        expr_node->add(repeatNode);
        if (first != last) ++first; // Move the iterator forward
        if (repeatNode->eval(first, last)) {
            expr_node->add(parse_expr(first, last));
        }
        return expr_node;
    }

    return nullptr;
}

/*<MATCH>                ->  <EXPR> */
match_op *parse_match(it &first, it last) {
    auto expr_node = parse_expr(first, last);
    if (!expr_node)
        return nullptr;
    auto result = new match_op;
    result->add(expr_node);
    return result;
}

/*     <GROUP>                ->  (  <EXPR>  ) */
group_op *parse_group(it &first, it last) {
    Lexer lexer;
    lexer.check(first, last);
    if (lexer.type == Lexer::GROUP_START) {
        ++first; // Move the iterator forward
        auto text_node = parse_text(first, last);
        if (!text_node) {
            return nullptr;
        }
        lexer.check(first, last);
        if (lexer.type != Lexer::GROUP_END) {
            std::cerr << "Expected ')' not found " << std::endl;
            return nullptr;
        }
        auto group_node = new group_op;
        group_node->add(text_node);
        return group_node;
    }
    return nullptr;
}



or_op *parse_or(it first, it last) {
    auto first_check = first;
    text_op *lhs = parse_text(first_check, last);
    if (!lhs) {
        std::cout << "lhs is nullptr\n";
        return nullptr;
    }
    auto token = Lexer::get_next_token(first_check, last);
    if (token != Lexer::OR) {
        std::cout << "Next token is not OR\n";
        return nullptr;
    }

    first_check++; // Skip the '+' operator
    text_op *rhs = parse_text(first_check, last);

    if (!rhs) {
        std::cout << "rhs is nullptr\n";
        return nullptr;
    }

    first = first_check;
    auto *result = new or_op;
    result->add(lhs);
    result->add(rhs);
    std::cout << "Parsed or operation: " << '\n';
    std::cout << "lhs: " << lhs << '\n';
    std::cout << "rhs: " << rhs << '\n';
    return result;
}

any_op *parse_any(it &first, it last) {
    Lexer lexer;
    lexer.check(first, last);
    if (lexer.type == Lexer::ANY) {
        auto *result = new any_op(*first); // Pass the current character to the any_op constructor
        ++first; // Move the iterator forward
        if (first != last && lexer.type == Lexer::ANY) {
            result->add(parse_any(first, last)); // Recursively call parse_any
        }
        return result;
    }
    return nullptr;
}

repeat_op *parse_repeat(it &first, it last) {
    Lexer lexer;
    lexer.check(first, last);
    if (lexer.type == Lexer::REPEAT) {
        ++first; // Move the iterator forward
        return new repeat_op;
    }
    return nullptr;
}



