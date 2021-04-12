#pragma once

namespace models
{
    class IObserver;

    class IObservable
    {
    public:
        virtual void attach(IObserver* observer) = 0;
        virtual void detatch(IObserver* observer) = 0;
        virtual void notify_observers() = 0;
    };
}
