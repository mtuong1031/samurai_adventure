#ifndef _TransformComponent_h_
#define _TransformComponent_h_

#include "Component.h" 
#include "../Vector2D.h "

/// quản lý và hiển thị các thành phần vị trí
class TransformComponent : public Component 
{
    public:
        Vector2D position;
        Vector2D velocity; // tốc độ

        int height = 32;
        int width = 32;
        int scale = 1;

        int speed = 3;

        TransformComponent() {
            position.Zero();
        }

        TransformComponent(int sc)
        {
            position.Zero();
            scale = sc;
        }

        TransformComponent(float x, float y) {
            position.Zero();
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
        void update() override {
            position.x += velocity.x * speed;
            position.y += velocity.y * speed;
        }
};


#endif // _TransformComponent_H_