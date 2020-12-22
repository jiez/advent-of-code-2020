Thoughts
========

General
-------

1. set in C++ is ordered. set does not have intersection or union method.
   It uses std::set\_intersection and std::set\_union.


Day 12
------

If a vector [a b]' turns clockwise A degree to get [x y]', we have

x = a cosA + b sinA

y = - a sinA + b cosA


Day 14
------

-Wconversion will be useful for catch conversion bugs.
It is not enabled by -Wall or -Wextra.


Day 17
------

How to Create a generic solution for 3-D and 4-D ?

How about using unordered_map? Will it improve performance?

Add constructor to some struct types to see if it can simplify code.


Day 19
------

I first converted rules into an NFA, then converted this NFA into a DFA,
then used this DFA to recognize the messages. There might be a better way to
solve these two puzzles of Day 19.


Day 22
------

Bug! Bug! Bug!


```
    for (int i = 0; i < cards1_sub.size() - num1; i++)
        cards1_sub.pop();
```

I chose queue at the beginning. But later it seems a bad decision for part 2.
We need to get the first several elements from the container.

We also need to print all elements in the container not destructively
for debug purpose.
