// Delegate.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Delegate.h"

#include <iostream>
#include <vector>

int Add(int _arg)
{
    std::cout << _arg << "\n";

    return ++_arg;
}

int Multiply2(int _arg)
{
    std::cout << _arg << "\n";

    return _arg * 2;
}

struct Math
{
    int Multiply4(int _arg)
    {
        std::cout << _arg << "\n";

        return _arg * 4;
    }
};


int main()
{
    std::vector<Delegate<int(int)>> _delegates;

    auto& _d1 = _delegates.emplace_back();
    _d1.Bind<&Add>();
    auto& _d2 = _delegates.emplace_back();
    _d2.Bind<&Multiply2>();

    
    Math m;

    auto& _d3 = _delegates.emplace_back();
    _d3.Bind<Math, &Math::Multiply4>(&m);

    int arg = 10;

    for (auto& _delegate : _delegates)
    {
        arg = _delegate.Invoke(arg);
    }


    std::cout << "Hello World!\n";
}