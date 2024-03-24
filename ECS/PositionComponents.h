#ifndef _POSITIONCOMPONENT_H_
#define _POSITIONCOMPONENT_H_

#include "Component.h" 


/// quản lý và hiển thị các thành phần vị trí
class PositionComponent : public Component {
    private:
        int xpos;
        int ypos;

    public:

        PositionComponent() {
            xpos = 0;
            ypos = 0;
        }

        PositionComponent(int x, int y) {
            xpos = x;
            ypos = y ;
        }

        void update() override {
            xpos++;
            ypos++;
        }

        int x() { return xpos; }
        void x(int x) { xpos = x; }
        int y() { return ypos; }   
        void y(int y) { ypos = y; }
        void SetPos(int x, int y) {
            xpos = x;
            ypos = y;
        }

};


#endif // _POSITIONCOMPONENT_H_