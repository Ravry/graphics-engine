#pragma once
#include <iostream>
#include <unordered_map>
#include <typeindex>
#include <memory>

class Component{
public:
    virtual ~Component() = default;
};

class EntityComponentSystem {
private:
    std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
public:
    template<typename T>
    void addComponent(T component) {
        components[std::type_index(typeid(T))] = std::make_unique<T>(component);
    }

    template<typename T>
    T* getComponent() {
        auto it = components.find(std::type_index(typeid(T)));
        if (it != components.end()) {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }

    template<typename T>
    void removeComponent() {
        components.erase(std::type_index(typeid(T)));
    }

    void printAllComponents() {
        for (auto& component : components) {
            std::cout << typeid(component.second.get()).name() << std::endl;
        }
    }
};