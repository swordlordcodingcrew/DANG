// (c) 2019-2021 by SwordLord - the coding crew
// This file is part of the DANG game framework
// Inspired by https://github.com/Sahnvour/PathFinder

#pragma once

#include "../DangFwdDecl.h"
#include "../RectT.hpp"
#include "Waypoint.hpp"

#include <vector>
#include <unordered_map>

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

        /** functions to create the scenegraph */
        void addWaypoint(uint32_t id, float x, float y, uint32_t type);
        void addNeighbour(uint32_t wp_id, uint32_t neigh_id, uint32_t type);
        void clearWaypoints();
        spSceneGraph split(const std::unordered_map<uint32_t, bool> &visited);

        /**
         * Returns the waypoints in the scenegraph
         * @return const-ref to the waypoint hashmap
         */
        const std::unordered_map<uint32_t, Waypoint>& getWaypoints() const { return _waypoints; }

        /**
         * find nearest waypoint from given pos. Connections are not checked.
         * This functions may be needed eg. if a sprite lost the path or to follow the hero
         * @param pos
         * @return waypoint pointer
         */
        const Waypoint* findNearestWaypoint(const Vector2F& pos);

        /**
         * find nearest waypoint from given pos (not checking connections) and return distance
         * This function is usually needed to find the closest waypoint in different graphs
         * @param pos position
         * @return distance from nearest waypoint to pos
         */
        float findNearestWaypointDist(const Vector2F& pos);

        /**
         * find nearest waypoint to the left or right of given rect
         * This functions is usually needed if a sprite lost the path
         * @param hotrect_abs
         * @return waypoint pointer
         */
        const Waypoint* findNearestWaypointH(const RectF& hotrect_abs);

        bool waypointReached(const RectF& hotrect_abs, const Waypoint* goal);

        bool getRandomNeighbourPath(const Waypoint* start, std::vector<const Waypoint*>& path);
        bool getRandomPath(const Waypoint* start, std::vector<const Waypoint*>& path);
        bool getNearestNeighbourHPath(const Waypoint* start, float dist, std::vector<const Waypoint*>& path);
        bool getNearestNeighbourPath(const Waypoint* start, float dist, std::vector<const Waypoint*>& path);

        const Waypoint* getWaypointWithType(uint32_t type);

        /**
         * calculates path (A*) from start to goal. The path is returned in @param path
         * @param start waypoint to start from
         * @param goal destination waypoint
         * @param path vector with the waypoints as a return value
         * @return true if a path was found. Else false
         */
        bool getPath(Waypoint* start, const Waypoint* goal, std::vector<const Waypoint*>& path);

        uint32_t getConnectionType(const Waypoint* start, const Waypoint* goal);

        /**
         * DFS search algo to check, if graph has disconnected components
         * @param start starting waypoint
         * @param visited each visited waypoint is set to true with key = id of waypoint
         */
        void dfsRecursion(const Waypoint* start, std::unordered_map<uint32_t, bool>& visited);


    protected:

        /** container of the waypoints */
        std::unordered_map<uint32_t, Waypoint> _waypoints;

        /**
         * Get a reandom waypoint from this scenegraph
         * @return waypoint pointer
         */
        const Waypoint* getRandomWaypoint();

        /**
         * Builds the path from the goal found back up to the start and reorders the order such that
         * the first entry is start. The function is used in getPath
         * @param wap goal
         * @param path the path in the correct order
         */
        void reconstructPath(Waypoint* wap, std::vector<const Waypoint*>& path);
        /** A* stuff */
        void resetAStar();
        void resetWaypoints();
        void pushOpen(Waypoint* wap);
        void popOpen(Waypoint* wap);
        std::vector<Waypoint*> open;
        std::vector<Waypoint*> closed;

        /**
         * Recursion function of dfs. See dfsRecursion
         * @param wp waypoint to continue
         * @param visited each visited waypoint is set to true with key = id of waypoint
         */
        void dfs(const Waypoint* wp, std::unordered_map<uint32_t, bool>& visited);

    };
}
