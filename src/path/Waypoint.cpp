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
    }

    Waypoint::Waypoint(uint32_t id, float x, float y, uint32_t type) :
        _f(0.0), _g(0.0), _h(0.0), _closed(false), _open(false),  _id(id), _pos(x, y), _type(type)
    {
    }

    Waypoint::~Waypoint()
    {
        /** the deletion of the waypoints is handled in the sceneGraph class */
//        std::cout << "destroying waypoint (" << _pos.x << "," << _pos.y << ")" << std::endl;
    }

    const std::vector<Waypoint::connection>& Waypoint::getNeighbours() const
    {
        return _neighbours;
    }

    void Waypoint::addNeighbour(Waypoint* child, connection &conn)
    {
        assert(child != nullptr);
        _neighbours.emplace_back(child, conn.distance, conn.type);
    }

    void Waypoint::addNeighbour(Waypoint* child, float distance, uint32_t type)
    {
        assert(child != nullptr);
        _neighbours.emplace_back(child, distance, type);
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
    float Waypoint::distanceTo(Waypoint* waypoint)
    {
        assert(waypoint != nullptr);
        return _pos.distance(waypoint->_pos);
    }

    void Waypoint::resetWaypoint()
    {
        _open = _closed = false;
        _f = _g = _h = 0.0f;
        _parent = nullptr;
    }

    const Waypoint* Waypoint::getNeighbour(size_t index) const
    {
        assert(index < _neighbours.size());
        return _neighbours[index].neighbour;
    }

    const Waypoint::connection& Waypoint::getNeighbourConnection(size_t index) const
    {
        assert(index < _neighbours.size());
        return _neighbours[index];
    }

}

