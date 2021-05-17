// (c) 2019-2021 by SwordLord - the coding crew
// This file is part of the DANG game framework
// Inspired by https://github.com/Sahnvour/PathFinder

#include <iostream>
#include "Waypoint.hpp"

namespace dang
{
    Waypoint::Waypoint() :
            _f(0.0), _g(0.0), _h(0.0), _closed(false), _open(false), _pos(0, 0)
    {
        _parent.reset();
    }

    Waypoint::~Waypoint()
    {
        std::cout << "destroying waypoint (" << _pos.x << "," << _pos.y << ")" << std::endl;
    }

    wpWaypoint Waypoint::getParent() const
    {
        return _parent;
    }

    std::vector<std::pair<spWaypoint, Waypoint::connection>>& Waypoint::getNeighbours()
    {
        return _neighbours;
    }

    void Waypoint::addNeighbour(spWaypoint child, connection &conn)
    {
        _neighbours.emplace_back(std::make_pair(child, conn));
    //	_children.push_back(std::make_pair(child,conn));
    }

    void Waypoint::clearNeighbours()
    {
        _neighbours.clear();
    }

    void Waypoint::setParent(wpWaypoint parent)
    {
        _parent = parent;
    }

    float Waypoint::distanceTo(spWaypoint wp)
    {
        return _pos.squareDistance(wp->_pos);
//        return _pos.distance(node->_pos);
    }

    void Waypoint::release()
    {
        _open = _closed = false;
        _f = _g = _h = 0.0f;
        _parent.reset();
        // TODO: correct?
        _neighbours.clear();
    }

}

