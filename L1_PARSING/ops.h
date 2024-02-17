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
    bool ignore_case = false;
    void set_ignore_case(bool ignore_case);

    std::vector<op *> children;
};

struct char_op : op {
    explicit char_op(char c) : character(c) {}

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
    int last_evaluated_child = -1; // -1 means neither child has been evaluated yet

};

struct any_op : char_op {
    bool eval(it first, it last) override;
    explicit any_op(const char c = '\0') : char_op(c) {}
};

struct repeat_op : op {
    bool eval(it first, it last) override;
};

struct ignore_case_op : op {

    bool eval(it first, it last) override;
    void ignore_case_for_all(op *node);

};

#endif //L1_PARSING_OPS_H
