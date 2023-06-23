// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <algorithm>
#include <cassert>
#include "Dispatcher.hpp"
#include "Event.hpp"

namespace dang
{
    void Dispatcher::removeSubscriber(uint32_t index)
    {
        _subscribers.erase(index);
    }

    uint32_t Dispatcher::registerSubscriber(std::function<void(Event &)> fn, uint16_t filter)
    {
        _index++;
        _subscriber_wrapper sw{fn, filter, _index};
        _subscribers.insert({_index, sw});
/*        _subscribers[_index] = _subscriber_wrapper();
        _subscribers[_index].fn = fn;
        _subscribers[_index].filt = filter;
        _subscribers[_index].ind = _index;
*/        return _index;

    }

    uint32_t Dispatcher::getIndex(const std::function<void(Event &)>& fn)
    {
        auto it = std::find_if(_subscribers.begin(), _subscribers.end(), [=](const std::pair<uint32_t, _subscriber_wrapper>& val)
                    {
                        auto ptr1 = val.second.fn.target<void(Event&)>();
                        auto ptr2 = fn.target<void(Event&)>();
                        return ( ptr1 == ptr2);
                    });

        // crappy way to check if the subscriber was found
        assert(it != _subscribers.end());

        return (*it).first;

    }

    void Dispatcher::removeSubscriber(const std::function<void(Event &)> &fn)
    {
        uint32_t ind = getIndex(fn);
        removeSubscriber(ind);
    }

    void Dispatcher::queueEvent(std::unique_ptr<Event> e)
    {
        _events.push(std::move(e));
    }

    void Dispatcher::publishEvents()
    {
        for(; !_events.empty(); _events.pop())
        {
            const auto& e = _events.front();

            for (std::pair<const uint32_t , _subscriber_wrapper>& pair : _subscribers)
            {
                if ((pair.second.filt & e->_filter) > 0)
                {
                    pair.second.fn(*e);
                }
            }
        }
    }
}