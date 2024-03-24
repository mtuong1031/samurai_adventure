#ifndef _ECS_H_
#define _ECS_H_

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;

using ComponentID = std::size_t;    


//Dùng để lưu trữ các component, 
inline ComponentID getComponentTypeID() {
    static ComponentID lastID = 0;
    return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept {
    static ComponentID typeID = getComponentTypeID();
    return typeID;
}

constexpr std::size_t maxComponents = 32;


using componentBitSet = std::bitset<maxComponents>; 
using componentArray = std::array<Component*, maxComponents>;

class Component {
    public:
        Entity* entity;

        virtual void init() {}
        virtual void update() {}
        virtual void draw() {}

        virtual ~Component() {}
};

class Entity 
{
    public:
        void update() {
            for(auto& c : components) c->update();
        }


        void Draw() {
            for (auto& c : components) c->draw();
        };
        bool isActive() const { return active; };
        void destroy() { active = false; };

        template <typename T> bool hasComponent() const {
            return componentBitset[getComponentTypeID<T>()];
        }

        template <typename T, typename... TArgs>
        T& addComponent(TArgs&&... mArgs) {
            T* c(new T(std::forward<TArgs>(mArgs)...));
            c->entity = this;
            std::unique_ptr<Component> uPtr{c};
            components.emplace_back(std::move(uPtr));

            componentArray[getComponentTypeID<T>()] = c;
            componentBitset[getComponentTypeID<T>()] = true;

            c->init();
            return *c;
        }

        template <typename T> T& getComponent() const {
            auto ptr(componentArray[getComponentTypeID<T>()]);
            return *static_cast<T*>(ptr);
        }

    private:
        bool active = true;
        std::vector<std::unique_ptr<Component>> components;
        componentArray componentArray;
        componentBitSet componentBitset;
};

class Manager 
{
    public:
        void update() {
            for (auto& e : entities) e->update();
        }

        void draw() {
            for (auto& e : entities) e->Draw();
        }

        void refresh() {
            entities.erase(std::remove_if(std::begin(entities), std::end(entities), [](const std::unique_ptr<Entity> &mEntity) {
                return !mEntity->isActive();
            }), 
                std::end(entities));
        }

        Entity& addEntity() {
            Entity* e = new Entity();
            std::unique_ptr<Entity> uPtr{ e };
            entities.emplace_back(std::move(uPtr));
            return *e;
        }
    private:
        std::vector<std::unique_ptr<Entity>> entities;
};

#endif // _ECS_H_
