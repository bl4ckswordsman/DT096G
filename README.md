# Theory of Programming Languages course - DT096G

 **Table of content:**
 - [Lab 1](#item-one)
 - [Lab 2](#item-two)
 - [Lab 3](#item-three)

<a id="item-one"></a>
## [Lab 1](/L1_PARSING): Parsing
```mermaid
classDiagram
op --> char_op
op --> text_op
op --> expr_op
op --> match_op
op --> group_op
op --> or_op
char_op --> any_op
char_op --> ignore_case_op
op : bool eval(it first, it last)
op : void add(op child)
op : vector<op *> children
char_op : bool eval(it first, it last) override
char_op : character char
text_op : bool eval(it first, it last) override
text_op : string text
expr_op : bool eval(it first, it last) override
match_op : bool eval(it first, it last) override
group_op : bool eval(it first, it last) override
or_op : bool eval(it first, it last) override
or_op : int last_evaluated_child
any_op : bool eval(it first, it last) override
ignore_case_op : bool eval(it first, it last) override
```

<a id="item-two"></a>
## [Lab 2](/L2_JS): Investigation of the Javascript prototype programming paradigm

<a id="item-three"></a>
## [Lab 3](/L3_CPP): Investigation of the C++ templating mechanism and implementation of an algorithm solved at compile time

<a id="item-four"></a>
## [Lab 4](/L4): ---

<br>

[![Visits Badge](https://badges.pufler.dev/visits/bl4ckswordsman/DT096G/)](https://github.com/bl4ckswordsman/DT096G/)
[![Hits](https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2Fbl4ckswordsman%2FDT096G&count_bg=%2379C83D&title_bg=%23555555&icon=&icon_color=%23E7E7E7&title=Daily+hits&edge_flat=false)](https://hits.seeyoufarm.com/api/count/graph/dailyhits.svg?url=https://github.com/bl4ckswordsman/DT096G) <!-- 2024-02-14 -->

