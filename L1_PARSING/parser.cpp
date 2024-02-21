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
    if (first == last) {
        return nullptr;
    }
    auto restore = first;

    ignore_case_op *ignore_caseNode = parse_ignore_case(first, last);
    if (ignore_caseNode) {
        auto expr_node = new expr_op;
        expr_node->add(ignore_caseNode);
        return expr_node;
    }



    auto group_op = parse_group(first, last);
    if (group_op) {
        auto expr_node = new expr_op;
        expr_node->add(group_op);
        if(first != last) {
            ++first;
        } // Move the iterator forward
        // Continue parsing the remaining operations
        expr_node->add(parse_expr(first, last));
        return expr_node;
    }

    /*or_op *orNode = parse_or(first, last);
    if (orNode) {
        auto expr_node = new expr_op;
        expr_node->add(orNode);
        if (first != last) ++first; // Move the iterator forward
        if (orNode->eval(first, last)) {
            expr_node->add(parse_expr(first, last));
        }
        return expr_node;
    }*/
    or_op *orNode = parse_or(first, last);
    if (orNode) {
        auto expr_node = new expr_op;
        expr_node->add(orNode);
        return expr_node;
    }

    any_op *anyNode = parse_any(first, last);
    if (anyNode) {
        auto expr_node = new expr_op;
        expr_node->add(anyNode);
        return expr_node;
    }

    auto text_node = parse_text(first, last);
    if (text_node) {
        auto expr_node = new expr_op;
        expr_node->add(text_node);
        return expr_node;
    }

    first = restore;


    repeat_op *repeatNode = parse_repeat(first, last);
    if (repeatNode) {
        auto expr_node = new expr_op;
        expr_node->add(repeatNode);
        return expr_node;
    }

    return nullptr;
}

op* parse_operand(it &first, it last) {
    auto restore = first;
    auto group_op = parse_group(first, last);
    if (group_op) {
        return group_op;
    }
    any_op *anyNode = parse_any(first, last);
    if (anyNode) {
        return anyNode;
    }
    auto text_node = parse_text(first, last);
    if (text_node) {
        return text_node;
    }
    /*first = restore;
    ignore_case_op *ignore_caseNode = parse_ignore_case(first, last);
    if (ignore_caseNode) {
        return ignore_caseNode;
    }*/
    repeat_op *repeatNode = parse_repeat(first, last);
    if (repeatNode) {
        return repeatNode;
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
op* parse_group(it &first, it last) {
    Lexer lexer;
    lexer.check(first, last);
    if (lexer.type == Lexer::GROUP_START) {
        ++first; // Move the iterator forward
        auto expr_node = parse_expr(first, last);
        if (!expr_node) {
            return nullptr;
        }
        lexer.check(first, last);
        while (first != last && lexer.type != Lexer::GROUP_END) {
            ++first;
            lexer.check(first, last);
        }
        lexer.check(first, last);
        if (lexer.type != Lexer::GROUP_END) {
            std::cerr << "Expected ')' not found " << std::endl;
            return nullptr;
        }
        ++first; // Move the iterator forward to check for ignore_case operation
        auto group_node = new group_op;
        group_node->add(expr_node);

        // Check for ignore case operation after the group operation
        ignore_case_op *ignore_caseNode = parse_ignore_case(first, last);
        if (ignore_caseNode) {
            ignore_caseNode->add(group_node);
            return ignore_caseNode;
        }

        return group_node;
    }
    return nullptr;
}



or_op *parse_or(it first, it last) {
    auto first_check = first;
    op *lhs = parse_operand(first_check, last);
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
    op *rhs = parse_operand(first_check, last);

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

/*ignore_case_op *parse_ignore_case(it &first, it last) {
    auto first_check = first;
    op *lhs = parse_text(first_check, last);
    if (!lhs) {
        std::cout << "lhs is nullptr\n";
        return nullptr;
    }
    auto token = Lexer::get_next_token(first_check, last);
    if (token != Lexer::IGNORE_CASE) {
        std::cout << "Next token is not IGNORE_CASE\n";
        return nullptr;
    }

    first_check++; // Skip the 'I' operator
    first_check++;

    first = first_check;
    auto *result = new ignore_case_op;
    result->add(lhs);

    std::cout << "Parsed ignore_case operation: " << '\n'; // TODO: Remove debug print
    std::cout << "lhs: " << lhs << '\n';
    return result;
}*/

ignore_case_op *parse_ignore_case(it &first, it last) {
    auto first_check = first;
    op *lhs = parse_operand(first_check, last); // Changed from parse_text to parse_operand
    if (!lhs) {
        std::cout << "lhs is nullptr\n";
        return nullptr;
    }
    auto token = Lexer::get_next_token(first_check, last);
    if (token != Lexer::IGNORE_CASE) {
        std::cout << "Next token is not IGNORE_CASE\n";
        return nullptr;
    }

    first_check++; // Skip the 'I' operator
    first_check++;

    first = first_check;
    auto *result = new ignore_case_op;
    result->add(lhs);

    std::cout << "Parsed ignore_case operation: " << '\n'; // TODO: Remove debug print
    std::cout << "lhs: " << lhs << '\n';
    return result;
}


