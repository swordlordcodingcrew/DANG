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
    class Waypoint
    {
    public:

        struct connection
        {
            connection(Waypoint* n, float d, uint32_t t) : neighbour(n), distance(d), type(t) {}
            Waypoint* neighbour;
            float distance;
            uint32_t type;
        };

        Waypoint();
        Waypoint(uint32_t id, float x, float y, uint32_t type);
        virtual ~Waypoint();

        Waypoint(const Waypoint& wp) = delete;  //!< copy constructor wouldn't work, since neighbours are raw pointers

        void addNeighbour(Waypoint* child, connection& nc);
        void addNeighbour(Waypoint* child, float distance, uint32_t type);
        const Waypoint* getNeighbour(size_t index) const;
        const std::vector<connection>& getNeighbours() const;
        const connection& getNeighbourConnection(size_t index) const;
        uint32_t getType() const { return _type; }

        float distanceTo(Waypoint* waypoint);

        void resetWaypoint();

        /** id of waypoint */
        const uint32_t _id;

        /** position of waypoint */
        const Vector2F _pos;

    protected:
        /** this section is for the A* algo only and can only be accessed by the friend classes declared below */
        friend class SceneGraph;
        friend struct CompareWaypoints;

        /** A* params and helper A* params*/
        float _f;
        float _g;
        float _h;
        bool _open;
        bool _closed;
        Waypoint* _parent{nullptr};

    protected:
        /** Clears the neighbours of the node. */
        void clearNeighbours();

        /** List of all the node's children. */
        std::vector<connection> _neighbours;

        /** added value type (i.e. crate depot) */
        uint32_t _type;

    };

}
