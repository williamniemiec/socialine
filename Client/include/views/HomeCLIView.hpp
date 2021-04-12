#pragma once
#include <iostream>
#include <list>
#include "../controllers/HomeCLIController.hpp"
#include "../models/User.hpp"
#include "../models/Notification.hpp"
#include "../models/IObserver.hpp"
#include "../models/IObservable.hpp"
#include "../views/IView.hpp"
#include "../../../Utils/Types.h"

namespace views
{
    class HomeCLIView : public IView
    {
    //-------------------------------------------------------------------------
    //      Attributes
    //-------------------------------------------------------------------------
    private:
        static const std::string TITLE;
        models::User* user;
        controllers::HomeCLIController* homeController;


    //-------------------------------------------------------------------------
    //      Constructor & Destructor
    //-------------------------------------------------------------------------
    public:
        HomeCLIView(controllers::HomeCLIController* homeController, models::User* user);
        virtual ~HomeCLIView();


    //-------------------------------------------------------------------------
    //      Methods
    //-------------------------------------------------------------------------
    public:
        virtual void render();
        virtual void update(models::IObservable* observable, std::list<std::string> data);

    private:
        void run();
    };
}
