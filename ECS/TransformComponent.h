#pragma once

#include "Component.h"
#include "../Vector2D.h "

/// quản lý và hiển thị các thành phần vị trí
class TransformComponent : public Component 
{
    public:
        Vector2D position;
        Vector2D velocity; // tốc độ

        int height = 19;
        int width = 19;
        int scale = 1;

        int speed = 7;

        TransformComponent() {
            position.Zero();
        }

        TransformComponent(int sc)
        {
            position.x = 30;
            position.y = 30;
            scale = sc;
        }

        TransformComponent(float x, float y) {
            position.x = x;
            position.y = y;
        }

        TransformComponent(float x, float y, int h, int w, int s) {
            position.x = x;
            position.y = y;
            height = h; 
            width = w;
            scale = s;
        }

        void init() override {
            velocity.Zero();
        }
        void update() override 
        {
            position.x += velocity.x * speed;
            position.y += velocity.y * speed;
        }
};

