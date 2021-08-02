// (c) 2019-2021 by SwordLord - the coding crew
// This file is part of the DANG game framework
// Inspired by https://github.com/Sahnvour/PathFinder

#pragma once

#include "../DangFwdDecl.h"
#include "../RectT.hpp"
#include "Waypoint.hpp"

#include <vector>
#include <map>

namespace dang
{
    struct CompareWaypoints
    {
        bool operator() (const Waypoint* wp1, const Waypoint* wp2) const
        {
            return wp1->_f < wp2->_f;
        }
    };

    class SceneGraph
    {
    public:

        SceneGraph();
        ~SceneGraph();

        void addWaypoint(uint32_t id, float x, float y, uint32_t type);
        void addNeighbour(uint32_t wp_id, uint32_t neigh_id, uint32_t type);
        void clearWaypoints();
        const std::unordered_map<uint32_t, Waypoint>& getWaypoints() const { return _waypoints; }
//        std::unordered_map<uint32_t, Waypoint>& getWaypoints() { return _waypoints; }
        spSceneGraph split(const std::unordered_map<uint32_t, bool> &visited);


        const Waypoint* getWaypointWithType(const uint32_t type);
        const Waypoint* findNearestWaypoint(const Vector2F& pos);
        const Waypoint* findNearestWaypointH(const RectF& hotrect_abs);
        bool waypointReached(const RectF& hotrect_abs, const Waypoint* goal);

        bool getRandomNextWaypoint(const Waypoint* start, std::vector<const Waypoint*>& path);
        bool getRandomPath(const Waypoint* start, std::vector<const Waypoint*>& path);
        uint32_t getConnectionType(const Waypoint* start, const Waypoint* goal);

        // A* algo
        bool getPath(Waypoint* start, const Waypoint* goal, std::vector<const Waypoint*>& path);

        // DFS search algo to check, if graph has disconnected components
        void dfsRecursion(const Waypoint* start, std::unordered_map<uint32_t, bool>& visited);
        void dfs(const Waypoint* wp, std::unordered_map<uint32_t, bool>& visited);

    protected:

        /** container of the waypoints */
        std::unordered_map<uint32_t, Waypoint> _waypoints;

        /** A* stuff */
        void resetAStar();
        void resetWaypoints();
        void pushOpen(Waypoint* wap);
        void popOpen(Waypoint* wap);
        std::vector<Waypoint*> open;
        std::vector<Waypoint*> closed;

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
        void reconstructPath(Waypoint* wap, std::vector<const Waypoint*>& path);

    };
}
