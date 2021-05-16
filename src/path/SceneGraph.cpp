// (c) 2019-2021 by SwordLord - the coding crew
// This file is part of the DANG game framework
// Inspired by https://github.com/Sahnvour/PathFinder

#include "SceneGraph.hpp"

namespace dang
{
    SceneGraph::SceneGraph()
    {

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

    void SceneGraph::releaseNodes()
    {
        for (const auto& wp : open)
        {
            if (spWaypoint spwp = wp.lock())
            {
                spwp->release();
            }
        }

        for (const auto& wp : closed)
        {
            if (spWaypoint spwp = wp.lock())
            {
                spwp->release();
            }
        }
    }

    void SceneGraph::clear()
    {
        releaseNodes();
        open.clear();
        closed.clear();
    }

}
