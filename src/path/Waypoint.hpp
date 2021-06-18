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

        void addNeighbour(wpWaypoint child, connection& nc);
        void addNeighbour(wpWaypoint child, float distance, uint32_t type);
        wpWaypoint getNeighbour(size_t index);
        std::vector<std::pair<wpWaypoint, connection>>& getNeighbours();
        const connection& getNeighbourConnection(size_t index);
        uint32_t getType() const { return _type; }

        float distanceTo(spWaypoint waypoint);

        void resetWaypoint();

        /** id of waypoint */
        const uint32_t _id;

        /** position of waypoint */
        const Vector2F _pos;

    protected:
        /** this section is for the A* algo only and can only be accessed by the friend classes decloared below */

        friend class SceneGraph;
        friend struct CompareWaypoints;

        /** A* params and helper A* params*/
        float _f;
        float _g;
        float _h;
        bool _open;
        bool _closed;
        wpWaypoint _parent;

    protected:

        /** Clears the neighbours of the node. */
        void clearNeighbours();

        /**
            List of all the node's children.
        */
        std::vector<std::pair<wpWaypoint, connection>> _neighbours;

        /** added value type */
        uint32_t _type;


    };

}
