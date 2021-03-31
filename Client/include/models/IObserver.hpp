#pragma once
#include <list>

namespace models
{
    class IObservable;

    class IObserver
    {
    public:
        virtual void update(IObservable* observable, std::list<std::string> data) = 0;
    };
}
