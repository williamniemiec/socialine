#pragma once
#include "../models/IObserver.hpp"

namespace views
{
    class IView : public wxFrame, public models::IObserver
    {
    //-------------------------------------------------------------------------
    //      Methods
    //-------------------------------------------------------------------------
    public:
        virtual void render() = 0;
    };
}
