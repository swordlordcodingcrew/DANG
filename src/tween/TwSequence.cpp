// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "TwSequence.hpp"

namespace dang
{

    TwSequence::TwSequence() : Tweenable()
    {
    }

    TwSequence::TwSequence(const TwSequence &tw) : Tweenable(tw), _index(tw._index)
    {
        for (auto it : tw._tw_seq)
        {
            _tw_seq.push_back(std::make_shared<Tweenable>(*it));
        }
    }

    void TwSequence::update(void* obj, uint32_t dt)
    {
        if (_tw_seq.empty() || _finished)
        {
            return;
        }

        _tw_seq.at(_index)->update(obj, dt);

        if (_tw_seq.at(_index)->isFinished())
        {
            _tw_seq.at(_index)->reset();

            _index++;
            if (_index == _tw_seq.size())       // the last tw has finished
            {
                _loop++;
                if (_loops < 0 || _loop < _loops)   // endless loop of twseq or not yet finished looping
                {
                    _index = 0;
                }
                else    // twseq finished
                {
                    _finished = true;
                    if (_finishedCB != nullptr)
                    {
                        _finishedCB();
                    }
                }
            }
/*            else    // next tw in twseq
            {
                _tw_seq.at(_index)->startTw(time);
            }
*/        }
    }

    void TwSequence::addTween(std::shared_ptr<Tweenable> tw)
    {
        _tw_seq.push_back(tw);
    }

    void TwSequence::finish(bool suppressCB)
    {
        for (const auto& it : _tw_seq)
        {
            it->finish(suppressCB);
        }
        Tweenable::finish(suppressCB);
        _tw_seq.clear();
    }

    void TwSequence::reset()
    {
        Tweenable::reset();
    }

    bool TwSequence::isFinished()
    {
        return Tweenable::isFinished();
    }

}