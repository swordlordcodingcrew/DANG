// (c) 2019-2021 by SwordLord - the coding crew
// This file is part of the DANG game framework
// Inspired by https://github.com/Sahnvour/PathFinder

#include <cfloat>
#include <iostream>
#include <engine/engine.hpp>
#include <cassert>
#include <src/TmxExtruder.hpp>
#include "SceneGraph.hpp"
#include "src/Vector2T.hpp"

namespace dang
{
    SceneGraph::SceneGraph()
    {
        std::srand(blit::now());
    }

    SceneGraph::~SceneGraph()
    {

    }

    bool SceneGraph::getPath(Waypoint* start, const Waypoint* goal, std::vector<const Waypoint*>& path)
    {
        assert(start != nullptr);
        assert(goal != nullptr);

        bool ret{false};

        Waypoint* currentNode;
        Waypoint* childNode;
        float f, g, h;

        std::make_heap(open.begin(), open.end(), CompareWaypoints());
        pushOpen(start);

        while (!open.empty())
        {
            std::sort(open.begin(), open.end(), CompareWaypoints());

            currentNode = open.front(); // pop n node from open for which f is minimal
            popOpen(currentNode);

            currentNode->_closed = true;
            closed.push_back(currentNode);

            if (currentNode == goal)
            {
                reconstructPath(currentNode, path);
                ret = true;
                break;
            }

            for (const Waypoint::connection& children : currentNode->getNeighbours())    // for each successor n' of n
            {
                childNode = children.neighbour;

                g = currentNode->_g + children.distance; // distance from start + distance between the two nodes
                if ((childNode->_open || childNode->_closed) && childNode->_g <  g) // n' is already in opend or closed with a lower cost g(n')
                {
                    continue; // consider next successor
                }

                h = children.distance; //distanceBetween(sp_childnode, sp_goal);
                f = g + h; // compute f(n')
                childNode->_f = f;
                childNode->_g = g;
                childNode->_h = h;
                childNode->_parent = currentNode;

                if (childNode->_closed)
                {
                    childNode->_closed = false;
                }

                if (!childNode->_open)
                {
                    pushOpen(childNode);
                }
            }
        }

        resetAStar();
        return ret;
    }

    void SceneGraph::reconstructPath(Waypoint* wap, std::vector<const Waypoint*>& path)
    {
        assert(wap != nullptr);
        std::vector<const Waypoint*> rpath;

        const Waypoint* parent = wap->_parent;
        rpath.push_back(wap);
        while (parent != nullptr)
        {
            rpath.push_back(parent);
            parent = parent->_parent;
        }

        for (auto rit = std::rbegin(rpath); rit != std::rend(rpath); ++rit)
        {
            path.push_back(*rit);
        }

    }

    void SceneGraph::pushOpen(Waypoint* wap)
    {
        open.push_back(wap);
        std::push_heap(open.begin(), open.end(), CompareWaypoints());
        wap->_open = true;
    }

    void SceneGraph::popOpen(Waypoint* wap)
    {
        std::pop_heap(open.begin(), open.end(), CompareWaypoints());
        open.pop_back();
        wap->_open = false;
    }

    void SceneGraph::resetWaypoints()
    {
        for (const auto& wp : open)
        {
            wp->resetWaypoint();
        }

        for (const auto& wp : closed)
        {
            wp->resetWaypoint();
        }
    }

    void SceneGraph::resetAStar()
    {
        resetWaypoints();
        open.clear();
        closed.clear();
    }

    void SceneGraph::addWaypoint(uint32_t id, float x, float y, uint32_t type)
    {
        _waypoints.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(id, x, y, type));
//        _waypoints.emplace(id, Waypoint(id, x, y, type));
    }

    void SceneGraph::addNeighbour(uint32_t wp_id, uint32_t neigh_id, uint32_t type)
    {
        if (_waypoints.count(wp_id) == 0 || _waypoints.count(neigh_id) == 0)
        {
            // fail gracefully. The connection does not belong to this scenegraph
            return;
        }

        Waypoint* wp = &_waypoints.at(wp_id);
        Waypoint* neighbour = &_waypoints.at(neigh_id);
        float dist = wp->distanceTo(neighbour);
        wp->addNeighbour(neighbour, dist, type);
    }

    void SceneGraph::clearWaypoints()
    {
        _waypoints.clear();
    }

    const Waypoint* SceneGraph::findNearestWaypoint(const Vector2F &pos)
    {
        float dist = FLT_MAX;
        const Waypoint* ret{nullptr};
        for (const auto& sp : _waypoints)
        {
            float newdist = sp.second._pos.squareDistance(pos);
            if (newdist < dist)
            {
                dist = newdist;
                ret = &sp.second;
            }
        }
        return ret;
    }

    float SceneGraph::findNearestWaypointDist(const Vector2F& pos)
    {
        float dist = FLT_MAX;
        for (const auto& sp : _waypoints)
        {
            float newdist = sp.second._pos.distance(pos);
            if (newdist < dist)
            {
                dist = newdist;
            }
        }
        return dist;
    }


    const Waypoint* SceneGraph::findNearestWaypointH(const RectF& hotrect_abs)
    {
        float dist = FLT_MAX;
        const Waypoint* ret{nullptr};
        for (const auto& sp : _waypoints)
        {
            // point is within vertical extent of hotrect
            if (hotrect_abs.containsV(sp.second._pos))
            {
                float newdist = sp.second._pos.squareDistance(hotrect_abs.center());
                if (newdist < dist)
                {
                    dist = newdist;
                    ret = &sp.second;
                }
            }
        }
        return ret;

    }

    bool SceneGraph::waypointReached(const RectF& hotrect_abs, const Waypoint* goal)
    {
        assert(goal != nullptr);
        if (hotrect_abs.contains(goal->_pos))
        {
            // fine tuning. wp within 5 pixels of hotrect-center
            if (std::abs(hotrect_abs.center().x - goal->_pos.x) < 2)
            {
                return true;
            }
        }
        return false;
    }

    bool SceneGraph::getRandomNextWaypoint(const Waypoint* start, std::vector<const Waypoint*> &path)
    {
        if (start->getNeighbours().empty())
        {
            return false;
        }

        size_t r = std::rand() % start->getNeighbours().size();
//            std::cout << "next index " << r << " - connection type " << wp->getNeighbourConnection(r).type << std::endl;
        path.push_back(start->getNeighbour(r));
        return true;
    }

    bool SceneGraph::getRandomPath(const Waypoint *start, std::vector<const Waypoint *> &path)
    {
        return false;
    }

    uint32_t SceneGraph::getConnectionType(const Waypoint* start, const Waypoint* goal)
    {
        if (start == nullptr || goal == nullptr)
        {
            return e_tmx_waypoint_connection::wpc_invalid;
        }

        for (const auto& neigh : start->getNeighbours())
        {
            if (neigh.neighbour == goal)
            {
                return neigh.type;
            }
        }

        return e_tmx_waypoint_connection::wpc_invalid;
    }

    const Waypoint* SceneGraph::getWaypointWithType(uint32_t type)
    {
        const Waypoint* ret{nullptr};
        for (const auto& sp : _waypoints)
        {
            if (sp.second.getType() == type)
            {
                ret = &(sp.second);
                break;
            }
        }
        return ret;
    }



    void SceneGraph::dfsRecursion(const Waypoint* start, std::unordered_map<uint32_t, bool>& visited)
    {
        dfs(start, visited);
    }

    void SceneGraph::dfs(const Waypoint* wp, std::unordered_map<uint32_t, bool> &visited)
    {
        visited[wp->_id] = true;
        for (const auto & neigh : wp->getNeighbours())
        {
            const Waypoint* child = neigh.neighbour;
            if (!visited.at(child->_id))
            {
                dfs(child,visited);
            }
        }

    }

    spSceneGraph SceneGraph::split(const std::unordered_map<uint32_t, bool> &visited)
    {
        spSceneGraph ret = std::make_shared<SceneGraph>();
        for (auto v : visited)
        {
            if (v.second)
            {
                Waypoint& wp = _waypoints[v.first];
                ret->addWaypoint(wp._id, wp._pos.x, wp._pos.y, wp._type);
                _waypoints.erase(v.first);
            }
        }
        return ret;
    }

}
