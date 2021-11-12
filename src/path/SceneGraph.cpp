// (c) 2019-2021 by SwordLord - the coding crew
// This file is part of the DANG game framework
// Inspired by https://github.com/Sahnvour/PathFinder

#include "src/TmxExtruder.hpp"
#include "src/Rand.hpp"
#include "SceneGraph.hpp"
#include "src/Vector2T.hpp"

#include <cfloat>
#include <iostream>
//#include <engine/engine.hpp>
#include <cassert>


namespace dang
{
    SceneGraph::SceneGraph()
    {
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
                // wpc_block counts as no connection
                if (children.type == wpc_block)
                {
                    continue;
                }

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
            // fine tuning. wp within 2 pixels of hotrect-center
            if (std::abs(hotrect_abs.center().x - goal->_pos.x) < 1)
            {
                return true;
            }
        }
        return false;
    }

    bool SceneGraph::getRandomNeighbourPath(const Waypoint* start, std::vector<const Waypoint*> &path)
    {
        if (start == nullptr)
        {
            return false;
        }

        if (start->getNeighbours().empty())
        {
            return false;
        }

        const Waypoint* wp{nullptr};
        size_t iter{0};
        while (wp == nullptr && iter < 100)
        {
            iter++;
            // some compilers seem to have a problem otherwise, explicit cast below
            uint32_t r = Rand::get((uint32_t )0, start->getNeighbours().size() - 1);
            if (start->getNeighbourConnection(r).type != wpc_block)
            {
                wp = start->getNeighbour(r);
            }
        }

        if (wp == nullptr)
        {
            std::cout << "ERROR: waypoint is a dead-end" << std::endl;
            return false;
        }

        path.push_back(wp);

        return true;
    }

    bool SceneGraph::getRandomPath(const Waypoint *start, std::vector<const Waypoint *> &path)
    {
        if (_waypoints.size() < 2)
        {
            // fail gracefully if there is only one waypoint (or less) in the graph
            return false;
        }

        const Waypoint* dest = getRandomWaypoint();
        while (start == dest)
        {
            dest = getRandomWaypoint();
        }

        assert(dest != nullptr);

        // alternatively to a const_cast we could fetch a non-const pointer from the _waypoint hashmap.
        // this would cost more cycles than just casting it. So...
        return getPath(const_cast<Waypoint*>(start), dest, path);
    }

    bool SceneGraph::getNearestNeighbourHPath(const Waypoint *start, float dist, std::vector<const Waypoint *> &path)
    {
        if (start->getNeighbours().empty())
        {
            return false;
        }

        float delta{0};
        const Waypoint* goal{nullptr};
        if (dist < 0)
        {
            delta = FLT_MIN;
            for (const auto& wpc : start->getNeighbours())
            {
                if (wpc.type != wpc_block)
                {
                    float pos_delta = start->_pos.x - wpc.neighbour->_pos.x;
                    float y_delta = std::abs(start->_pos.y - wpc.neighbour->_pos.y);
                    if (pos_delta < 0 && y_delta < 10)
                    {
                        float newdelta = std::abs(dist - pos_delta);
                        if (newdelta > delta)
                        {
                            goal = wpc.neighbour;
                            delta = newdelta;
                        }
                    }
                }
            }
        }
        else if (dist > 0)
        {
            delta = FLT_MAX;
            for (const auto& wpc : start->getNeighbours())
            {
                if (wpc.type != wpc_block)
                {
                    float pos_delta = start->_pos.x - wpc.neighbour->_pos.x;
                    float y_delta = std::abs(start->_pos.y - wpc.neighbour->_pos.y);
                    if (pos_delta > 0 && y_delta < 10)
                    {
                        float newdelta = std::abs(dist - pos_delta);
                        if (newdelta < delta)
                        {
                            goal = wpc.neighbour;
                            delta = newdelta;
                        }
                    }
                }
            }
        }

        if (goal == nullptr)
        {
            return false;
        }
        path.push_back(goal);
        return true;
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
            if (sp.second.getType() & type)
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

    const Waypoint *SceneGraph::getRandomWaypoint()
    {
        // some compilers seem to have a problem otherwise, explicit cast below
        uint32_t r = Rand::get((uint32_t )0, _waypoints.size()-1);
//        size_t r = std::rand() % _waypoints.size();
        size_t i{0};
        const Waypoint* ret{nullptr};
        for (const auto& w : _waypoints)
        {
            ret = &w.second;
            if (i == r)
            {
                break;
            }
            ++i;
        }
        return ret;
    }


}
