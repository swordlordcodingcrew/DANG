// (c) 2019-2021 by SwordLord - the coding crew
// This file is part of the DANG game framework
// Inspired by https://github.com/Sahnvour/PathFinder

#pragma once

#include <vector>
#include <cstdint>
#include <src/Vector2T.hpp>
#include <memory>

namespace dang
{
    class Waypoint;
    using spWaypoint = std::shared_ptr<Waypoint>;
    using wpWaypoint = std::weak_ptr<Waypoint>;


    class Waypoint
    {
    public:

        struct connection
        {
            float distance;
            uint32_t type;
        };


        Waypoint();
        Waypoint(uint32_t id, float x, float y, uint32_t type);
        virtual ~Waypoint();

        void setParent(wpWaypoint parent);

        wpWaypoint getParent() const;

        void addNeighbour(wpWaypoint child, connection& nc);
        void addNeighbour(wpWaypoint child, float distance, uint32_t type);
        wpWaypoint getNeighbour(size_t index);


        std::vector<std::pair<wpWaypoint, connection>>& getNeighbours();


//        void setPosition(Vector2F& pos) { _pos = pos; };
//        void setPosition(float x, float y) { _pos.x = x; _pos.y = y; }
        void setF(float f) { _f = f; }
        void setG(float g) { _g = g; }
        void setH(float h) { _h = h; }
        void setOpen(bool value) { _open = value; }
        void setClosed(bool value) { _closed = value; }

        inline float getX() const { return _pos.x; }
        inline float getY() const { return _pos.y; }
        inline float getF() const { return _f; }
        inline float getG() const { return _g; }
        inline float getH() const { return _h; }
        inline bool isOpen() const { return _open; }
        inline bool isClosed() const { return _closed; }

        float distanceTo(spWaypoint waypoint);

        void resetWaypoint();

        /** id of waypoint */
        const uint32_t _id;

        /** position of waypoint */
        const Vector2F _pos;

    protected:

        /** Clears the neighbours of the node. */
        void clearNeighbours();

        /** Pointer to the parent node. */
        wpWaypoint _parent;

        /**
            List of all the node's children.
        */
        std::vector<std::pair<wpWaypoint, connection>> _neighbours;

        /** added value type */
        uint32_t _type;

        /** A* params */
        float _f;
        float _g;
        float _h;
        bool _open;
        bool _closed;

    };

}
