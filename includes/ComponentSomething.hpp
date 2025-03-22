#pragma once
#include <iostream>
#include "EntityComponentSystem.hpp"

class ComponentSomething : public Component  {
private:
public:

    void doSomething() {
        std::cout << "doing something!" << std::endl;
    }
};