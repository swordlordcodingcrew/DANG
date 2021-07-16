// (c) 2019-2021 by SwordLord - the coding crew
// This file is part of the DANG game framework
// Inspired by https://github.com/Sahnvour/PathFinder

#pragma once

#include <vector>
#include <algorithm>
#include <map>
#include <src/RectT.hpp>
#include "Waypoint.hpp"

namespace dang
{
    struct CompareWaypoints
    {
        bool operator() (const wpWaypoint wap1, const wpWaypoint wap2) const
        {
            if (auto spwap1 = wap1.lock())
            {
                if (auto spwap2 = wap2.lock())
                {
                    return spwap1->_f < spwap2->_f;
                }
            }
            // this case should not happen
            return false;
        }
    };

    class SceneGraph
    {
    public:

        SceneGraph();
        ~SceneGraph();

        void addWaypoint(uint32_t id, spWaypoint wp);
        void clearWaypoints();
        std::map<uint32_t, spWaypoint>& getWaypoints() { return _waypoints; }

        spWaypoint getWaypointWithType(const uint32_t type);
        spWaypoint findNearestWaypoint(const Vector2F& pos);
        spWaypoint findNearestWaypointH(const RectF hotrect_abs);
        bool waypointReached(const RectF hotrect_abs, wpWaypoint goal);

        bool getRandomNextWaypoint(wpWaypoint start, std::vector<wpWaypoint>& path);
        uint32_t getConnectionType(wpWaypoint start, wpWaypoint goal);

        // A* algo
        bool getPath(wpWaypoint start, wpWaypoint goal, std::vector<wpWaypoint>& path);

    protected:

        /** container of the waypoints */
        std::map<uint32_t, spWaypoint> _waypoints;

        /** A* stuff */
        void resetAStar();
        void resetWaypoints();
        void pushOpen(wpWaypoint wap);
        void popOpen(wpWaypoint wap);
        std::vector<wpWaypoint> open;
        std::vector<wpWaypoint> closed;

        /**
            @brief Computes the distance between two nodes using the specified
            Node::distanceTo() method from T.
            @param[in] n1 A pointer referencing the source node.
            @param[in] n2 A pointer referencing the destination node.
            @see Node::distanceTo()
        inline float distanceBetween(spWaypoint wap1, spWaypoint wap2) const
        {
            return wap1->distanceTo(wap2);
        }
*/

        /**
            @brief Builds the path from the goal found back up to the start and reorders the order such that
            the first entry is start. The function is used in getPath
        */
        void reconstructPath(wpWaypoint wap, std::vector<wpWaypoint>& path);

    };
}
