// (c) 2019-2021 by SwordLord - the coding crew
// This file is part of the DANG game framework
// Inspired by https://github.com/Sahnvour/PathFinder

#include <cfloat>
#include <iostream>
#include <engine/engine.hpp>
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

    bool SceneGraph::getPath(wpWaypoint start, wpWaypoint goal, std::vector<wpWaypoint>& path)
    {
        bool ret = false;
        spWaypoint sp_goal = goal.lock();
        if (sp_goal)
        {
            wpWaypoint currentNode, childNode;
            spWaypoint sp_curnode, sp_childnode;
            float f, g, h;

            std::make_heap(open.begin(), open.end(), CompareWaypoints());
            pushOpen(start);

            while (!open.empty())
            {
                std::sort(open.begin(), open.end(), CompareWaypoints());

                currentNode = open.front(); // pop n node from open for which f is minimal
                sp_curnode = currentNode.lock();
                if (sp_curnode)
                {
                    popOpen(currentNode);

                    sp_curnode->_closed = true;
                    closed.push_back(currentNode);

                    if (sp_curnode == sp_goal)
                    {
                        reconstructPath(currentNode, path);
                        ret = true;
                        break;
                    }

                    for (const auto& children : sp_curnode->getNeighbours())    // for each successor n' of n
                    {
                        childNode = children.first;
                        sp_childnode = childNode.lock();
                        if (sp_childnode)
                        {
                            g = sp_curnode->_g + children.second.distance; // distance from start + distance between the two nodes
                            if ((sp_childnode->_open || sp_childnode->_closed) && sp_childnode->_g <  g) // n' is already in opend or closed with a lower cost g(n')
                            {
                                continue; // consider next successor
                            }

                            h = children.second.distance; //distanceBetween(sp_childnode, sp_goal);
                            f = g + h; // compute f(n')
                            sp_childnode->_f = f;
                            sp_childnode->_g = g;
                            sp_childnode->_h = h;
                            sp_childnode->_parent = currentNode;

                            if (sp_childnode->_closed)
                            {
                                sp_childnode->_closed = false;
                            }

                            if (!sp_childnode->_open)
                            {
                                pushOpen(childNode);
                            }
                        }
                    }
                }
            }
        }

        resetAStar();
        return ret;
    }

    void SceneGraph::reconstructPath(wpWaypoint wap, std::vector<wpWaypoint>& path)
    {
        spWaypoint sp_wap = wap.lock();
        std::vector<wpWaypoint> rpath;

        if (sp_wap)
        {
            wpWaypoint parent = sp_wap->_parent;
            spWaypoint sp_par;
            rpath.push_back(wap);
            while (!parent.expired())
            {
                rpath.push_back(parent);
                sp_par = parent.lock();
                parent = sp_par->_parent;
            }

            for (auto rit = std::rbegin(rpath); rit != std::rend(rpath); ++rit)
            {
                path.push_back(*rit);
            }
        }

    }

    void SceneGraph::pushOpen(wpWaypoint wap)
    {
        spWaypoint sp_wap = wap.lock();
        if (sp_wap)
        {
            open.push_back(wap);
            std::push_heap(open.begin(), open.end(), CompareWaypoints());
            sp_wap->_open = true;
        }
    }

    void SceneGraph::popOpen(wpWaypoint wap)
    {
        spWaypoint sp_wap = wap.lock();
        if (sp_wap)
        {
            std::pop_heap(open.begin(), open.end(), CompareWaypoints());
            open.pop_back();
            sp_wap->_open = false;
        }
    }

    void SceneGraph::resetWaypoints()
    {
        for (const auto& wp : open)
        {
            if (spWaypoint spwp = wp.lock())
            {
                spwp->resetWaypoint();
            }
        }

        for (const auto& wp : closed)
        {
            if (spWaypoint spwp = wp.lock())
            {
                spwp->resetWaypoint();
            }
        }
    }

    void SceneGraph::resetAStar()
    {
        resetWaypoints();
        open.clear();
        closed.clear();
    }

    void SceneGraph::addWaypoint(uint32_t id, spWaypoint wp)
    {
        _waypoints[id] = wp;
    }

    void SceneGraph::clearWaypoints()
    {
        _waypoints.clear();
    }

    spWaypoint SceneGraph::findNearestWaypoint(const Vector2F &pos)
    {
        float dist = FLT_MAX;
        spWaypoint ret;
        for (auto sp : _waypoints)
        {
            float newdist = sp.second->_pos.squareDistance(pos);
            if (newdist < dist)
            {
                dist = newdist;
                ret = sp.second;
            }
        }
        return ret;
    }

    spWaypoint SceneGraph::findNearestWaypointH(const RectF hotrect_abs)
    {
        float dist = FLT_MAX;
        spWaypoint ret;
        for (auto sp : _waypoints)
        {
            // point is within vertical extent of hotrect
            if (hotrect_abs.containsV(sp.second->_pos))
            {
                float newdist = sp.second->_pos.squareDistance(hotrect_abs.center());
                if (newdist < dist)
                {
                    dist = newdist;
                    ret = sp.second;
                }
            }
        }
        return ret;

    }

    bool SceneGraph::waypointReached(const RectF hotrect_abs, wpWaypoint goal)
    {
        spWaypoint wp = goal.lock();
        if (wp)
        {
            if (hotrect_abs.contains(wp->_pos))
            {
                // fine tuning. wp within 5 pixels of hotrect-center
                if (std::abs(hotrect_abs.center().x - wp->_pos.x) < 2)
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool SceneGraph::getRandomNextWaypoint(wpWaypoint start, std::vector<wpWaypoint> &path)
    {
        spWaypoint wp = start.lock();
        if (wp)
        {
            size_t r = std::rand() % wp->getNeighbours().size();
//            std::cout << "next index " << r << " - connection type " << wp->getNeighbourConnection(r).type << std::endl;
            path.push_back(wp->getNeighbour(r));
            return true;
        }
        return false;
    }

    uint32_t SceneGraph::getConnectionType(wpWaypoint start, wpWaypoint goal)
    {
        spWaypoint st = start.lock();
        spWaypoint go = goal.lock();
        if (!st || !go)
        {
            return 0;
        }

        for (auto neigh : st->getNeighbours())
        {
            if (neigh.first.lock() == go)
            {
                return neigh.second.type;
            }
        }

        return 0;
    }

    spWaypoint SceneGraph::getWaypointWithType(uint32_t type)
    {
        spWaypoint ret;
        for (auto sp : _waypoints)
        {
            if (sp.second->getType() == type)
            {
                ret = sp.second;
                break;
            }
        }
        return ret;
    }


}
