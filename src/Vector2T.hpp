// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
// (c) 2019-20 by SwordLord - the coding crew
//
// inspired by sfml, oxygine and 32blit

#pragma once

#include <cmath>

namespace dang
{
    template <typename T>
    class Vector2T
    {
    public:
        union { T x; T w;};
        union { T y; T h;};
//        T x, y;

        Vector2T()
            : x(0), y(0)
        {
        }

        Vector2T(T xIn, T yIn)
                : x(xIn), y(yIn)
        {
        }

        template<typename U>
        explicit Vector2T(const Vector2T<U> &vector)
            : x(static_cast<T>(vector.x)), y(static_cast<T>(vector.y))
        {
        }

/*        Vector2T<T>&    operator =(const Vector2T<T>& right)
        {
            x = right.x;
            y = right.y;
            return *this;
        }
*/


        void normalize()
        {
            T d = this->length();
            if (d > 0)
            {
                x /= d;
                y /= d;
            }
        }

        bool empty()
        {
            return x == 0 && y == 0;
        }

        T length()
        {
            if (x == 0) return std::abs(y);
            if (y == 0) return std::abs(x);
            return std::sqrt(x * x + y * y);
        }

        T cross(const Vector2T<T> &a)
        {
            return x * a.y - y * a.x;
        }

        T dot(const Vector2T<T> &a)
        {
            return (x * a.x) + (y * a.y);
        }

        void rotate(const T &a)
        {
            float c = std::cos(a);
            float s = std::sin(a);
            float rx = this->x * c - this->y * s;
            float ry = this->x * s + this->y * c;
            this->x = rx;
            this->y = ry;
        }


        T angle(Vector2T<T> o)
        {
            //atan2d(x1*y2-y1*x2,x1*x2+y1*y2);
            return std::atan2(this->x * o.y - this->y * o.x, this->x * o.x + this->y * o.y);
            //return acos(this->dot(o));
        }

    };


    template <typename T>
    Vector2T<T>     operator -(const Vector2T<T>& right)
    {
        return Vector2T<T>(-right.x, -right.y);
    }

    template <typename T>
    Vector2T<T>     operator +(const Vector2T<T>& left, const Vector2T<T>& right)
    {
        return Vector2T<T>(left.x + right.x, left.y + right.y);
    }

    template <typename T>
    Vector2T<T>     operator -(const Vector2T<T>& left, const Vector2T<T>& right)
    {
        return Vector2T<T>(left.x - right.x, left.y - right.y);
    }

    template <typename T>
    Vector2T<T>     operator *(const Vector2T<T>& left, T right)
    {
        return Vector2T<T>(left.x * right, left.y * right);
    }

    template <typename T>
    Vector2T<T>     operator *(T left, const Vector2T<T>& right)
    {
        return Vector2T<T>(left * right.x, left * right.y);
    }

    template <typename T>
    Vector2T<T>&    operator +=(Vector2T<T>& left, const Vector2T<T>& right)
    {
        left.x += right.x;
        left.y += right.y;

        return left;
    }

    template <typename T>
    Vector2T<T>&    operator -=(Vector2T<T>& left, const Vector2T<T>& right)
    {
        left.x -= right.x;
        left.y -= right.y;

        return left;
    }

    template <typename T>
    Vector2T<T>&    operator *=(Vector2T<T>& left, T right)
    {
        left.x *= right;
        left.y *= right;

        return left;
    }

    template <typename T>
    Vector2T<T>     operator /(const Vector2T<T>& left, T right)
    {
        return Vector2T<T>(left.x / right, left.y / right);
    }

    template <typename T>
    Vector2T<T>&    operator /=(Vector2T<T>& left, T right)
    {
        left.x /= right;
        left.y /= right;

        return left;
    }

    template <typename T>
    bool operator ==(const Vector2T<T>& left, const Vector2T<T>& right)
    {
        return (left.x == right.x) && (left.y == right.y);
    }

    template <typename T>
    bool operator !=(const Vector2T<T>& left, const Vector2T<T>& right)
    {
        return (left.x != right.x) || (left.y != right.y);
    }

    typedef Vector2T<int>          Vector2I;
    typedef Vector2T<unsigned int> Vector2U;
    typedef Vector2T<float>        Vector2F;
    typedef Vector2T<double>       Vector2D;

    typedef Vector2T<int>          PointI;
    typedef Vector2T<unsigned int> PointU;
    typedef Vector2T<float>        PointF;
    typedef Vector2T<double>       PointD;

    typedef Vector2T<int>          SizeI;
    typedef Vector2T<unsigned int> SizeU;
    typedef Vector2T<float>        SizeF;
    typedef Vector2T<double>       SizeD;

}   // namespace dang


