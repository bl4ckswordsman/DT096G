//
// Created by amarildo on 2024-01-26.
//

#include "parser.h"
#include <iostream>
#include "Lexer.h"

char_op *parse_char(it &first, it last) {
    Lexer lexer;
    auto character = lexer.check(first, last);
    if (lexer.type == Lexer::END)
        return nullptr;
    return new char_op(character);
}

/*text_op* parse_text(it& first, const it& last) {
    auto* text = new text_op("");
    while (first != last && isalnum(*first) || isspace(*first)) {
        text->add(new char_op(*first));
        ++first;
    }
    return text;
}*/

text_op *parse_text(it &first, it last) {
    if (first == last) {
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

/*op* parse_expr(std::string input, const std::string& text) {
    auto start = input.begin();
    auto end = input.end();
    op* result = nullptr;
    if (input.find('+') != std::string::npos) {
        result = parse_or(start, end);
        if (result) {
            auto text_start = input.begin();
            auto text_end = input.end();
            if (result->eval(text_start, text_end)) {
                std::cout << "Parsed or operation: " << '\n';
            }
        }
    } else {
        result = parse_text(start, end);
        auto text_start = input.begin();
        auto text_end = input.end();
        if (result && result->eval(text_start, text_end)) {
            std::cout << "Parsed word operation: " << dynamic_cast<text_op*>(result)->get_text() << '\n';
        }
    }

    return result;
}*/

expr_op *parse_expr(it &first, it last) {
    auto restore = first;
    auto group_op = parse_group(first, last);
    if (group_op) {
        auto expr_node = new expr_op;
        expr_node->add(group_op);
        expr_node->add(parse_expr(first, last));
        return expr_node;
    }
    auto text_node = parse_text(first, last);
    if (text_node) {
        auto expr_node = new expr_op;
        expr_node->add(text_node);
        expr_node->add(parse_expr(first, last));
        return expr_node;
    }
    return nullptr;
}

match_op *parse_match(it &first, it last) {
    auto expr_node = parse_expr(first, last);
    if (!expr_node)
        return nullptr;
    auto result = new match_op;
    result->add(expr_node);
    return result;
}

group_op *parse_group(it &first, it last) {
    Lexer lexer;
    auto ch = lexer.check(first, last);
    if (lexer.type == Lexer::GROUP_START) {
        auto text_node = parse_text(first, last);
        if (!text_node)
            return nullptr;
        lexer.check(first, last);
        if (lexer.type != Lexer::GROUP_END) {
            return nullptr;
        }
        auto group_node = new group_op;
        group_node->add(text_node);
        return group_node;
    }
    return nullptr;
}


/*
or_op* parse_or(char *&first, char *last) {    bool eval(const char* &first, const char* &last) override;

    word_op* lhs = parse_word(first, last);
    std::cout << "lhs: " << (lhs ? lhs->word : "nullptr") << '\n';
    if (!lhs) {
        return nullptr;
    }
    Lexer lexer;
    auto token = lexer.get_next_token(first, last);
    std::cout << "token.type: " << token.type << '\n';
    if (token.type != Lexer::OR) {
        return nullptr;
    }
    ++first; // Skip the '+' operator
    word_op* rhs = parse_word(first, last);
    std::cout << "rhs: " << (rhs ? rhs->word : "nullptr") << '\n';
    if (!rhs) {
        std::cerr << "Usage: <word> + <word>\n" << std::endl;
        return nullptr;
    }
    auto *result = new or_op;
    result->operands.push_back(lhs);
    result->operands.push_back(rhs);
    return result;
} */

or_op *parse_or(it first, it last) {
    text_op *lhs = parse_text(first, last);
    if (!lhs) {
        return nullptr;
    }

    auto tok = Lexer::get_next_token(first, last);
    if (tok != Lexer::OR) {
        return nullptr;
    }
    if (first == last) {
        return nullptr;
    }
    text_op *rhs = parse_text(first, last);
    if (!rhs) {
        return nullptr;
    }
    ++first;
    auto *result = new or_op;
    result->add(lhs);
    result->add(rhs);
    return result;
}

or_op *parse_match(it first, it last) {
    or_op *operand = parse_or(first, last);
    if (!operand) {
        return nullptr;
    }
    return operand;
}

bool search_word(const std::vector<char_op *> &word, std::string text) {
    auto start = text.begin();
    auto end = text.end();
    for (auto &w: word) {
        if (!w->eval(start, end)) {
            return false;
        }
    }
    return true;
}
