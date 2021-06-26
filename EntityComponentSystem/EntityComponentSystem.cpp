// EntityComponentSystem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "ECS.h"

struct ComponentA
{
    int a, b, c;
};

struct ComponentB
{
    double a, b, c;
};


int main()
{
    ECS::Scene _Scene;

    auto _Entity1 = _Scene.CreateEntity();
    auto _Entity2 = _Scene.CreateEntity();

    _Scene.Assign<ComponentA>(_Entity1);
    _Scene.Assign<ComponentB>(_Entity1);

    _Scene.Remove<ComponentA>(_Entity1);

    _Scene.DestroyEntity(_Entity1);


    auto _Entity3 = _Scene.CreateEntity();
    _Scene.Assign<ComponentB>(_Entity3);


    std::cout << "Hello World!\n";
}