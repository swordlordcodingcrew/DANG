// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once
#include <memory>
#include <unordered_map>
#include <forward_list>
#include <functional>
#include <queue>

namespace dang
{
    class Event;

    /**
     * simple pub-sub-class. Idea & most code is based on EventDispatcher in the oxyggine framework
     *
     * */
    class Dispatcher
    {
    public:
        void queueEvent(std::unique_ptr<Event> e);
        void publishEvents();

//        void publishEvent(Event& e);

        uint32_t registerSubscriber(std::function<void(Event&)> fn, uint16_t filter = 0xffff);
        uint32_t    getIndex(const std::function<void(Event&)>& fn);
        void removeSubscriber(uint32_t index);
        void removeSubscriber(const std::function<void(Event&)>& fn);

//        void removeSubscriber(const std::shared_ptr<Subscriber>& s);
//        uint32_t registerSubscriber(std::weak_ptr<Subscriber> s, uint16_t filter = 0xffff);


    protected:

        struct _subscriber_wrapper
        {
            std::function<void(Event&)> fn;
            uint16_t filt{0xffff};
            uint32_t ind{0};
        };

        std::unordered_map<uint32_t, _subscriber_wrapper> _subscribers;
        uint32_t _index{0};

        // remove me after push
        //std::forward_list<std::unique_ptr<Event>> _event_list;
        //std::forward_list<std::unique_ptr<Event>> _event_list_filler;

        std::queue<std::unique_ptr<Event>> _events;
    };

}

