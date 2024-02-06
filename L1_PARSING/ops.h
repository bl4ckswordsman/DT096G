//
// Created by amarildo on 2024-01-30.
//

#ifndef L1_PARSING_OPS_H
#define L1_PARSING_OPS_H

#include <string>
#include <vector>

using it = std::string::iterator;

struct op {
    virtual bool eval(it first, it last);

    void add(op *child) {
        if (child) {
            children.push_back(child);
        }
    }

    std::vector<op *> children;
};

struct char_op : op {
    explicit char_op(char c) : character(c) {
    }

    bool eval(it first, it last) override;

    char character;
};

struct text_op : op {
    //explicit text_op(const std::string &word);
    bool eval(it first, it last) override;
};

struct expr_op : op {
    bool eval(it first, it last) override;
};

struct match_op : op {
    bool eval(it first, it last) override;
};

struct group_op : op {
    bool eval(it first, it last) override;
};

struct or_op : op {
    bool eval(it first, it last) override;
};

struct any_op : op {
    bool eval(it first, it last) override;
};

#endif //L1_PARSING_OPS_H
