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
        spWaypoint sp_goal = goal.lock();
        if (!sp_goal)
        {
            return false;
        }

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

                sp_curnode->setClosed(true);
                closed.push_back(currentNode);

                if (sp_curnode == sp_goal)
                {
                    reconstructPath(currentNode, path);
                    return true;
                }

                for (const auto& children : sp_curnode->getNeighbours())    // for each successor n' of n
                {
                    childNode = children.first;
                    sp_childnode = childNode.lock();
                    if (sp_childnode)
                    {
                        g = sp_curnode->getG() + children.second.distance; // distance from start + distance between the two nodes
                        if ((sp_childnode->isOpen() || sp_childnode->isClosed()) && sp_childnode->getG() <  g) // n' is already in opend or closed with a lower cost g(n')
                        {
                            continue; // consider next successor
                        }

                        h = distanceBetween(sp_childnode, sp_goal);
                        f = g + h; // compute f(n')
                        sp_childnode->setF(f);
                        sp_childnode->setG(g);
                        sp_childnode->setH(h);
                        sp_childnode->setParent(currentNode);

                        if (sp_childnode->isClosed())
                        {
                            sp_childnode->setClosed(false);
                        }

                        if (!sp_childnode->isOpen())
                        {
                            pushOpen(childNode);
                        }
                    }
                }
            }
        }
        return false;
    }

    void SceneGraph::reconstructPath(wpWaypoint wap, std::vector<wpWaypoint>& path)
    {
        spWaypoint sp_wap = wap.lock();
        std::vector<wpWaypoint> rpath;

        if (sp_wap)
        {
            wpWaypoint parent = sp_wap->getParent();
            spWaypoint sp_par;
            rpath.push_back(wap);
            while (!parent.expired())
            {
                rpath.push_back(parent);
                sp_par = parent.lock();
                parent = sp_par->getParent();
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
            sp_wap->setOpen(true);
        }
    }

    void SceneGraph::popOpen(wpWaypoint wap)
    {
        spWaypoint sp_wap = wap.lock();
        if (sp_wap)
        {
            std::pop_heap(open.begin(), open.end(), CompareWaypoints());
            open.pop_back();
            sp_wap->setOpen(false);
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

    spWaypoint SceneGraph::getNearestWaypoint(const Vector2F &pos)
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


}
