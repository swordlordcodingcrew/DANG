// (c) 2019-2021 by SwordLord - the coding crew
// This file is part of the DANG game framework
// Inspired by https://github.com/Sahnvour/PathFinder

#include <iostream>
#include <cassert>
#include "Waypoint.hpp"

namespace dang
{
    Waypoint::Waypoint() :
            _f(0.0), _g(0.0), _h(0.0), _closed(false), _open(false), _pos(0, 0), _id(0), _type(0)
    {
        _parent.reset();
    }

    Waypoint::Waypoint(uint32_t id, float x, float y, uint32_t type) :
        _f(0.0), _g(0.0), _h(0.0), _closed(false), _open(false),  _id(id), _pos(x, y), _type(type)
    {
        _parent.reset();
    }

    Waypoint::~Waypoint()
    {
//        std::cout << "destroying waypoint (" << _pos.x << "," << _pos.y << ")" << std::endl;
    }

/*    wpWaypoint Waypoint::getParent() const
    {
        return _parent;
    }
*/
    std::vector<std::pair<wpWaypoint, Waypoint::connection>>& Waypoint::getNeighbours()
    {
        return _neighbours;
    }

    void Waypoint::addNeighbour(wpWaypoint child, connection &conn)
    {
        _neighbours.push_back(std::make_pair(child, conn));
    }

    void Waypoint::addNeighbour(wpWaypoint child, float distance, uint32_t type)
    {
        connection co = {distance, type};
        _neighbours.push_back(std::make_pair(child, co));
    }

    void Waypoint::clearNeighbours()
    {
        _neighbours.clear();
    }

/*    void Waypoint::setParent(wpWaypoint parent)
    {
        _parent = parent;
    }
*/
    float Waypoint::distanceTo(spWaypoint waypoint)
    {
        return _pos.distance(waypoint->_pos);
    }

    void Waypoint::resetWaypoint()
    {
        _open = _closed = false;
        _f = _g = _h = 0.0f;
        _parent.reset();
    }

    wpWaypoint Waypoint::getNeighbour(size_t index)
    {
        assert(index < _neighbours.size());

        return _neighbours[index].first;
    }

    const Waypoint::connection& Waypoint::getNeighbourConnection(size_t index)
    {
        assert(index < _neighbours.size());

        return _neighbours[index].second;
    }

}

