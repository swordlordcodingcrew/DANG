// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "TwSequence.h"

namespace dang
{

    TwSequence::TwSequence() : Tweenable()
    {
    }

    void TwSequence::update(uint32_t time)
    {
        if (_tw_seq.empty() || _state == TW_FINISHED)
        {
            return;
        }

        _tw_seq.at(_index)->update(time);

        if (_tw_seq.at(_index)->isTwFinished())
        {
            _tw_seq.at(_index)->resetTw();

            _index++;
            if (_index == _tw_seq.size())       // the last tw has finished
            {
                _loop++;
                if (_loops < 0 || _loop < _loops)   // endless loop of twseq or not yet finished looping
                {
                    _index = 0;
                    _start_time = time;
                }
                else    // twseq finished
                {
                    _state = TW_FINISHED;
                    if (_finishedCB != nullptr)
                    {
                        _finishedCB();
                    }
                }
            }
            else    // next tw in twseq
            {
                _tw_seq.at(_index)->startTw(time);
            }
        }
    }

    void TwSequence::setObject(std::weak_ptr<void> obj)
    {
        for (const auto& it : _tw_seq)
        {
            if (it)
            {
                it->setObject(obj);
            }
        }
    }

    void TwSequence::addTween(std::shared_ptr<Tweenable> tw)
    {
        _tw_seq.push_back(tw);
    }

    void TwSequence::startTw(uint32_t start_time)
    {
        if (_state == TW_READY && !_tw_seq.empty())
        {
            Tweenable::startTw(start_time);
            _tw_seq.front()->startTw(start_time);
        }
    }

    void TwSequence::pauseTw(uint32_t time)
    {
        Tweenable::pauseTw(time);
        if (!_tw_seq.empty())
        {
            _tw_seq.front()->pauseTw(time);
        }
    }

    void TwSequence::continueTw(uint32_t time)
    {
        Tweenable::continueTw(time);
        if (!_tw_seq.empty())
        {
            _tw_seq.front()->continueTw(time);
        }

    }

    void TwSequence::finishTw(bool suppressCB)
    {
        for (const auto& it : _tw_seq)
        {
            it->finishTw(suppressCB);
        }
        Tweenable::finishTw(suppressCB);
        _tw_seq.clear();
    }

    void TwSequence::resetTw()
    {
        Tweenable::resetTw();

    }

    bool TwSequence::isTwFinished()
    {
        return Tweenable::isTwFinished();
    }
}