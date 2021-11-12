// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
// (c) 2019-20 by SwordLord - the coding crew
//
// inspired by sfml and oxygine

#pragma once

#include "Vector2T.hpp"
#include <algorithm>
#include <cstdint>

namespace dang
{
    template <typename T>
    class RectT
    {
    public:
        // variables
        T x, y, w, h;

        RectT()
                : x(0), y(0), w(0), h(9)
        {
        }

        RectT(T xIn, T yIn, T wIn, T hIn)
                : x(xIn), y(yIn), w(wIn), h(hIn)
        {
        }

        RectT(Vector2T<T> pos, Vector2T<T> size)
                : x(pos.x), y(pos.y), w(size.x), h(size.y)
        {
        }

        template <typename U>
        explicit RectT(const RectT<U>& rect)
                : x(static_cast<T>(rect.x)), y(static_cast<T>(rect.y)), w(static_cast<T>(rect.w)), h(static_cast<T>(rect.h))
        {
        }

        T area() const
        {
            return w * h;
        }

        bool containsWOBounds(const Vector2T<T> &p) const
        {
            return p.x > x && p.y > y && p.x < x + w && p.y < y + h;
        }

        bool containsZeroPoint() const
        {
//            return x <= 0 && (x + w) >= 0 && y <= 0 && (y + h) >= 0;
            return x < 0 && (x + w) > 0 && y < 0 && (y + h) > 0;
        }

        bool contains(const Vector2T<T> &p) const
        {
            return p.x >= x && p.y >= y && p.x < x + w && p.y < y + h;
        }

        bool containsH(const Vector2T<T> &p) const
        {
            return p.x >= x && p.x < x + w;
        }

        bool containsV(const Vector2T<T> &p) const
        {
            return p.y >= y && p.y < y + h;
        }

        bool contains(const RectT<T> &r) const
        {
            return r.x >= x && r.y >= y && r.x + r.w < x + w && r.y + r.h < y + h;
//            return r.x > x && r.y > y && r.x + r.w < x + w && r.y + r.h < y + h;
        }

        bool intersects(const RectT<T> &r) const
        {
            return !(x > r.x + r.w || x + w < r.x || y > r.y + r.h || y + h < r.y);
        }


        RectT<T> intersection(const RectT<T> &r) const
        {
            return RectT<T>(
                    std::max(x, r.x),
                    std::max(y, r.y),
                    std::min(x + w, r.x + r.w) - std::max(x, r.x),
                    std::min(y + h, r.y + r.h) - std::max(y, r.y));
        }

        // calculates the minkowsky difference between 2 rects, which is another rect
        RectT<T> minkowskiDiff(const RectT<T> &r) const
        {
//            RectT<T> ret = {x - r.x - r.w, r.y - y - h, w + r.w, h + r.h};
            RectT<T> ret = {x - r.x - r.w, y - r.y - r.h, w + r.w, h + r.h};
//            RectT<T> ret = {r.x - x - w, r.y - y - h, w + r.w, h + r.h};
            return ret;
        }

        Vector2T<T> getNearestCorner(const Vector2T<T> p)
        {
            return Vector2T<T>(std::abs(x - p.x) < std::abs(x + w - p.x) ? x : x + w, std::abs(y - p.y) < std::abs(y + h - p.y) ? y : y + h);
        }

        void deflate(T v)
        {
            x += v;
            y += v;
            w -= 2 * v;
            h -= 2 * v;
        }


        void inflate(T v)
        {
            x -= v;
            y -= v;
            w += 2 * v;
            h += 2 * v;
        }


        Vector2T<T> clamp(Vector2T<T> p) const
        {
            return Vector2T<T>(
                    p.x < x ? x : (p.x > x + w ? x + w : p.x),
                    p.y < y ? y : (p.y > y + h ? y + h : p.y));
        }


        Vector2T<T> tl() const
        {
            return Vector2T<T>(x, y);
        }


        Vector2T<T> tr() const
        {
            return Vector2T<T>(x + w, y);
        }


        Vector2T<T> bl() const
        {
            return Vector2T<T>(x, y + h);
        }


        Vector2T<T> br() const
        {
            return Vector2T<T>(x + w, y + h);
        }

        Vector2T<T> center() const
        {
            return Vector2T<T>(x + w / 2, y + h / 2);
        }

        T top() const
        {
            return y;
        }

        T bottom() const
        {
            return y + h;
        }

        T left() const
        {
            return x;
        }

        T right() const
        {
            return x + w;
        }
    };

    template <typename T>
    bool        operator ==(const RectT<T>& left, const RectT<T>& right)
    {
        return (left.x == right.x &&
                left.y == right.y &&
                left.w == right.w &&
                left.h == right.h);
    }

    template <typename T>
    bool        operator !=(const RectT<T>& left, const RectT<T>& right)
    {
        return (left.x != right.x ||
                left.y != right.y ||
                left.w != right.w ||
                left.h != right.h);
    }

    template <typename T>
    RectT<T>&    operator *=(RectT<T>& left, const T right)
    {
        left.x *= right;
        left.y *= right;
        left.w *= right;
        left.h *= right;
        return left;
    }

    template <typename T>
    RectT<T>     operator *(const RectT<T>& left, T right)
    {
        return RectT<T>(left.x * right, left.y * right, left.w * right, left.h * right);
    }

    template <typename T>
    RectT<T>     operator *(T left, const RectT<T>& right)
    {
        return RectT<T>(right.x * left, right.y * left, right.w * left, right.h * left);
    }


    typedef RectT<int32_t>      RectI;
    typedef RectT<uint32_t>     RectU;
    typedef RectT<float>        RectF;
    typedef RectT<double>       RectD;

} // namespace dang

