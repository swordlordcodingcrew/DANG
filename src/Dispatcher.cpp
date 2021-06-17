// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <algorithm>
#include "Dispatcher.hpp"
#include "Event.hpp"

namespace dang
{

/*    void Dispatcher::publishEvent(Event &e)
    {
        for (std::pair<const unsigned int, _subscriber_wrapper>& pair : _subscribers)
        {
            if ((pair.second.filt & e._filter) > 0)
            {
                pair.second.fn(e);

                if (e._consumed)
                {
                    break; // for-loop
                }
            }
        }
    }
*/
    void Dispatcher::removeSubscriber(uint32_t index)
    {
        _subscribers.erase(index);
    }

    uint32_t Dispatcher::registerSubscriber(std::function<void(Event &)> fn, uint16_t filter)
    {
        _index++;
        _subscribers[_index] = _subscriber_wrapper();
        _subscribers[_index].fn = fn;
        _subscribers[_index].filt = filter;
        _subscribers[_index].ind = _index;
        return _index;

    }

    uint32_t Dispatcher::getIndex(const std::function<void(Event &)>& fn)
    {
//        try
//        {
            auto it = std::find_if(_subscribers.begin(), _subscribers.end(), [=](const std::pair<uint32_t, _subscriber_wrapper>& val)
            {
                auto ptr1 = val.second.fn.target<void(Event&)>();
                auto ptr2 = fn.target<void(Event&)>();
                return ( ptr1 == ptr2);
            });
/*        }
        catch (std::out_of_range &oor)
        {
            // do nothing
        }
        return 0;
*/    }

    void Dispatcher::removeSubscriber(const std::function<void(Event &)> &fn)
    {
        uint32_t ind = getIndex(fn);
        removeSubscriber(ind);
    }

    void Dispatcher::queueEvent(std::unique_ptr<Event> e)
    {
        _event_list.push_front(std::move(e));
    }

    void Dispatcher::publishEvents()
    {
        for (const auto& e : _event_list)
        {
            for (std::pair<const uint32_t , _subscriber_wrapper>& pair : _subscribers)
            {
                if ((pair.second.filt & e->_filter) > 0)
                {
                    pair.second.fn(*e);

                    if (e->_consumed)
                    {
                        break; // for-loop
                    }
                }
            }

        }

        if (!_event_list.empty())
        {
            _event_list.clear();
        }
    }

}