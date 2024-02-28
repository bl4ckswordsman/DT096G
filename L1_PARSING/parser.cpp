//
// Created by amarildo on 2024-01-26.
//

#include "parser.h"
#include <iostream>
#include "Lexer.h"

/*<CHAR>                  ->  any non-special character */
char_op *parse_char(it &first, it last) {
    auto restore = first;
    Lexer lexer;
    lexer.check(first, last);
    if (lexer.type == Lexer::END || lexer.type != Lexer::CHAR) {
        return nullptr;
    }
    ++first;
    lexer.check(first, last);
    if (lexer.type == Lexer::COUNT_START) {
        auto count_node = parse_count(first, last);
        if (count_node) {
            auto *result = new char_op(*restore);
            result->count = count_node->count;
            return result;
        }
    }
    first = restore;

    return new char_op(*first);
}


/*<TEXT>    ->  <CHAR> [<TEXT>] */
text_op *parse_text(it &first, it last) {
    if (first == last ) {
        return nullptr; // Base case to stop the recursion
    }
    auto char_node = parse_char(first, last);
    if (!char_node) {
        return nullptr;
    }
    auto *result = new text_op;
    result->add(char_node);
    if (first != last) {
        ++first;
        Lexer lexer;
        int type = lexer.check(first, last);

        if (type == Lexer::ANY) {
            auto any_node = parse_any(first, last);
            if (any_node) {
                result->add(any_node);
            }
        } else if (type == Lexer::STAR) {
            auto star_node = parse_star(first, last);
            if (star_node) {
                result->add(star_node);
            }
        } else {
            result->add(parse_text(first, last));
        }
    }
    return result;
}

/*<EXPR>      ->  <OR>  |  <STAR>  |  <GROUP>  |  <ANY>  |  <COUNT>  |  <IGNORE_CASE>  |  <OUTPUT>  |  <TEXT> */
/*expr_op *parse_expr(it &first, it last) {
    if (first == last) {
        return nullptr;
    }
    auto *exprNode = new expr_op;

    while (first != last) {
        if (output_op *outputNode = parse_output(first, last)) {
            exprNode->add(outputNode);
        } else if (ignore_case_op *ignore_caseNode = parse_ignore_case(first, last)) {
            exprNode->add(ignore_caseNode);
        } else if (count_op *countNode = parse_count(first, last)) {
            exprNode->add(countNode);
        } else if (op *groupNode = parse_group(first, last)) {
            exprNode->add(groupNode);
        } else if (or_op *orNode = parse_or(first, last)) {
            exprNode->add(orNode);
        } else if (text_op *anyNode = parse_any(first, last)) {
            exprNode->add(anyNode);
        } else if (text_op *textNode = parse_text(first, last)) {
            exprNode->add(textNode);
        } else if (star_op *repeatNode = parse_star(first, last)) {
            exprNode->add(repeatNode);
        } else {
            break; // If none of the operations match, break the loop
        }
    }

    return exprNode;
}*/

/**  <EXPR>                    ->  <OR>  |  <REPEAT>  |  <GROUP>  |
 *  <ANY>  |  <COUNT>  |  <IGNORE_CASE>  |  <OUTPUT>  |  <TEXT>     */
expr_op *parse_expr(it &first, it last) {
    if (first == last) {
        return nullptr;
    }
    auto restore = first;

    output_op *outputNode = parse_output(first, last);
    if (outputNode) {
        auto *exprNode = new expr_op;
        exprNode->add(outputNode);
        return exprNode;
    }

    ignore_case_op *ignore_caseNode = parse_ignore_case(first, last);
    if (ignore_caseNode) {
        auto expr_node = new expr_op;
        expr_node->add(ignore_caseNode);
        return expr_node;
    }

    count_op *countNode = parse_count(first, last);
    if (countNode) {
        auto expr_node = new expr_op;
        expr_node->add(countNode);
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


    or_op *orNode = parse_or(first, last);
    if (orNode) {
        auto expr_node = new expr_op;
        expr_node->add(orNode);
        return expr_node;
    }

    first = restore;
    text_op *anyNode = parse_any(first, last);
    if (anyNode) {
        auto expr_node = new expr_op;
        expr_node->add(anyNode);
        return expr_node;
    }

    auto text_node = parse_text(first, last);
    if (text_node) {
        auto expr_node = new expr_op;
        expr_node->add(text_node);
        expr_node->add(parse_expr(first, last));
        return expr_node;
    }

    first = restore;


    star_op *repeatNode = parse_star(first, last);
    if (repeatNode) {
        auto expr_node = new expr_op;
        expr_node->add(repeatNode);
        return expr_node;
    }

    return nullptr;
}

op* parse_basic_opration(it &first, it last) {
    auto restore = first;
    auto group_op = parse_group(first, last);
    if (group_op) {
        return group_op;
    }
    text_op *anyNode = parse_any(first, last);
    if (anyNode) {
        return anyNode;
    }
    auto text_node = parse_text(first, last);
    if (text_node) {
        return text_node;
    }

    star_op *star_node = parse_star(first, last);
    if (star_node) {
        return star_node;
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

        // Parse post group operations
        parse_post_group_operations(group_node, first, last);

        return group_node;
    }
    return nullptr;
}


void parse_post_group_operations(op* group_node, it &first, it last) {
    // Check for ignore case operation after the group operation
    ignore_case_op *ignore_caseNode = parse_ignore_case(first, last);
    if (ignore_caseNode) {
        group_node->add(ignore_caseNode);
    }

    // Check for any operation after the group operation
    text_op *anyNode = parse_any(first, last);
    if (anyNode) {
        group_node->add(anyNode);
    }
}


/**  <OR>                    ->  <EXPR>  +  <EXPR> */
or_op *parse_or(it first, it last) {
    auto first_check = first;
    op *lhs = parse_basic_opration(first_check, last);
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
    op *rhs = parse_basic_opration(first_check, last);

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

/**  <ANY>                    ->  .              */
text_op *parse_any(it &first, it last) {
    Lexer lexer;
    lexer.check(first, last);
    if (lexer.type == Lexer::ANY) {
        auto *any_node = new any_op(*first); // Pass the current character to the any_op constructor
        ++first; // Move the iterator forward
        lexer.check(first, last);
        if (first != last && lexer.type == Lexer::COUNT_START) {
            auto *count_node = parse_count(first, last);
            if (count_node) {
                any_node->count = count_node->count;
            }
        }
        // Wrap the any_op inside a text_op before returning (only needed for easier printing)
        auto *text_node = new text_op;
        text_node->add(any_node);
        lexer.check(first, last);
        if (first != last && lexer.type == Lexer::ANY) {
            text_node->add(parse_any(first, last)); // Recursively call parse_any
        }
        return text_node;
    }
    return nullptr;
}

/**  <STAR>               ->  <TEXT>  * */
star_op *parse_star(it &first, it last) {
    Lexer lexer;
    lexer.check(first, last);
    if (lexer.type == Lexer::STAR) {
        ++first; // Move the iterator forward
        return new star_op;
    }
    return nullptr;
}

/**  <IGNORE_CASE>    ->  <TEXT> \I  |  <GROUP> \I **/
ignore_case_op *parse_ignore_case(it &first, it last) {
    auto first_check = first;
    op *lhs = parse_basic_opration(first_check, last); // Changed from parse_text to parse_operand
    if (!lhs) {
        std::cout << "lhs is nullptr\n";
        return nullptr;
    }
    auto token = Lexer::get_next_token(first_check, last);
    if (token != Lexer::IGNORE_CASE) {
        std::cout << "Next token is not IGNORE_CASE\n";
        return nullptr;
    }

    first_check++; // Skip the '//I' operator
    first_check++; // Skip the 'I' character

    first = first_check;
    auto *result = new ignore_case_op;
    result->add(lhs);

    std::cout << "Parsed ignore_case operation: " << '\n'; // TODO: Remove debug print
    std::cout << "lhs: " << lhs << '\n';
    return result;
}

int parse_number(it &first, it last) {
    int number = 0;
    while (first != last && isdigit(*first)) {
        number = number * 10 + (*first - '0');
        ++first;
    }
    return number;
}

/** <COUNT>               ->  <CHAR>  {  <NUMBER>  } |  <ANY>  {  <NUMBER>  } **/
count_op *parse_count(it &first, it last) {
    Lexer lexer;
    if (lexer.check(first, last) == Lexer::COUNT_START) {
        ++first; // Move the iterator forward
        int count = parse_number(first, last);        // Parse the number enclosed in '{}'
        std::cout << "Parsed count: " << count << '\n'; //TODO: Remove debug print
        if (lexer.check(first, last) != Lexer::COUNT_END) {
            std::cerr << "Expected '}' not found " << std::endl;
            return nullptr;
        }
        ++first; // Move the iterator forward
        return new count_op(count);
    }
    return nullptr;
}

output_op *parse_output(it &first, it last) {
    Lexer lexer;
    if (lexer.check(first, last) == Lexer::OUTPUT) {
        ++first; // Skip the '\\'
        ++first; // Skip the 'O'
        if (lexer.check(first, last) == Lexer::COUNT_START) {
            auto count_node = parse_count(first, last);
            if (count_node) {
                return new output_op(count_node->count);
            }
        }
    }
    return nullptr;
}



