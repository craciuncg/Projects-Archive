#pragma once

template<typename A, typename B>
class Pair
{
public:
    A first;
    B second;

    Pair(A first, B second) : first(first), second(second) {}
    virtual ~Pair() = default;
};

