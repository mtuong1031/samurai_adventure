#ifndef _Vector2D_h_
#define _Vector2D_h_

#include <iostream>

// quản lí các phép toán trên vector 2 chiều
class Vector2D
{
    public:
        float x;
        float y;

        Vector2D();
        Vector2D(float x, float y);

        // các phép toán trên vector 2 chiều
        Vector2D& Add(const Vector2D& vec);
        Vector2D& Subtract(const Vector2D& vec);
        Vector2D& Multiply(const Vector2D& vec);
        Vector2D& Divide(const Vector2D& vec);

        friend Vector2D& operator+(Vector2D& v1, const Vector2D& v2);
        friend Vector2D& operator-(Vector2D& v1, const Vector2D& v2);
        friend Vector2D& operator*(Vector2D& v1, const Vector2D& v2);
        friend Vector2D& operator/(Vector2D& v1, const Vector2D& v2);

        Vector2D& operator+=(const Vector2D& vec);
        Vector2D& operator-=(const Vector2D& vec);
        Vector2D& operator*=(const Vector2D& vec);
        Vector2D& operator/=(const Vector2D& vec);

        Vector2D& operator*(const int& i);
        Vector2D& Zero();

    friend std::ostream& operator<<(std::ostream& stream, const Vector2D& vec);

};

#endif // _Vector2D_h_