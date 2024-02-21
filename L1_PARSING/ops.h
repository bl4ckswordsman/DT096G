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
    virtual std::string get_type() const = 0;

    void add(op *child);

    bool ignore_case = false;
    void set_ignore_case(bool ignore_case);
    explicit op (int count = 1) : count(count) {}

    std::vector<op *> children;
    int count = 1;
};

struct char_op : op {
    explicit char_op(char c) : character(c) {}

    bool eval(it first, it last) override;
    std::string get_type() const override { return "char_op"; }

    char character;
};

struct text_op : op {
    //explicit text_op(const std::string &word);
    bool eval(it first, it last) override;
    std::string get_type() const override { return "text_op"; }
};

struct expr_op : op {
    bool eval(it first, it last) override;
    std::string get_type() const override { return "expr_op"; }
};

struct match_op : op {
    bool eval(it first, it last) override;
    std::string get_type() const override { return "match_op"; }
};

struct group_op : op {
    bool eval(it first, it last) override;
    std::string get_type() const override { return "group_op"; }
};

struct or_op : op {
    bool eval(it first, it last) override;
    std::string get_type() const override { return "or_op"; }
    int last_evaluated_child = -1; // -1 means neither child has been evaluated yet

};

struct any_op : char_op {
    bool eval(it first, it last) override;
    std::string get_type() const override { return "any_op"; }
    explicit any_op(const char c = '\0') : char_op(c) {}
};

struct repeat_op : op {
    bool eval(it first, it last) override;
    std::string get_type() const override { return "repeat_op"; }
};

struct ignore_case_op : op {

    bool eval(it first, it last) override;
    std::string get_type() const override { return "ignore_case_op"; }
    void ignore_case_for_all(op *node);

};

struct subexpr_op : expr_op {
    bool eval(it first, it last) override;
    std::string get_type() const override { return "subexpr_op"; }
    explicit subexpr_op(op* child) {
        add(child);
    }
};

struct count_op : op {
    bool eval(it first, it last) override;
    std::string get_type() const override { return "count_op"; }


    count_op() = default;

    explicit count_op(int count = 1) : op(count) {}
};

#endif //L1_PARSING_OPS_H
