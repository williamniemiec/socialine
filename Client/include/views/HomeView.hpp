#pragma once
#include <iostream>
#include <list>
#include <wx/textdlg.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/statbmp.h>
#include <wx/listbox.h>
#include "../controllers/HomeController.hpp"
#include "../models/User.hpp"
#include "../models/Notification.hpp"
#include "../models/IObserver.hpp"
#include "../models/IObservable.hpp"
#include "../views/IView.hpp"
#include "../../../Utils/Types.h"

namespace views
{
    class HomeView : public IView
    {
    //-------------------------------------------------------------------------
    //      Attributes
    //-------------------------------------------------------------------------
    private:
        static const std::string TITLE;
        static const int WIDTH;
        static const int HEIGHT;
        static const int TITLE_HEIGHT;
        static const int HEADER_HEIGHT;
        static const int SEND_BTN_WIDTH;
        static const int NOTIFICATIONS_AREA_HEIGHT;
        static const int INPUT_WIDTH;
        static const int CONSOLE_AREA_HEIGHT;
        static const long ID_HEADER_IMG;
        static const long ID_PNL_HEADER;
        static const long ID_LBL_NOTIFICATIONS;
        static const long ID_NOTIFICATIONS_TITLE;
        static const long ID_NOTIFICATIONS_LIST;
        static const long ID_PNL_NOTIFICATIONS_BODY;
        static const long ID_LBL_CONSOLE;
        static const long ID_PNL_CONSOLE_TITLE;
        static const long ID_CONSOLE_LOG_LIST;
        static const long ID_PNL_CONSOLE_BODY;
        static const long ID_INPUT_MESSAGE;
        static const long ID_BTN_SEND;
        static const long ID_PNL_FOOTER;
        static const long ID_PNL_MAIN;
        static const long ID_MENU_QUIT;
        static const long ID_MENU_ABOUT;
        static const long ID_MENU_FOLLOW;
        static const long ID_IN_FOLLOW_USERNAME;
        models::User* user;
        controllers::HomeController* homeController;
        wxWindow* parent;
        wxListBox* notificationsList;
        wxListBox* consoleList;
        wxTextEntryDialog* dgFollowUsername;
        wxTextCtrl* inMessage;
        wxPanel* pnlMain;
        wxFlexGridSizer* flexGridSizerMain;


    //-------------------------------------------------------------------------
    //      Constructor & Destructor
    //-------------------------------------------------------------------------
    public:
        HomeView(controllers::HomeController* homeController, models::User* user,
                 wxWindow* parent,wxWindowID id = -1);
        virtual ~HomeView();


    //-------------------------------------------------------------------------
    //      Methods
    //-------------------------------------------------------------------------
    public:
        virtual void render();
        virtual void update(models::IObservable* observable, std::list<std::string> data);

    private:
        void build_window();
        void build_icon();
        void build_header();
        void build_header_background(wxPanel* panel);
        wxBitmap build_header_image();
        wxPanel* build_header_panel();
        void build_body();
        void build_body_panel();
        void build_notifications_area();
        void build_notifications_title();
        wxPanel* build_notifications_title_panel();
        void build_notifications_label(wxPanel* panel);
        void render_notifications_title(wxPanel* panel);
        void build_notifications_body();
        wxPanel* build_notifications_body_panel();
        void build_notifications_list(wxPanel* panel);
        void render_notifications_body(wxPanel* panel);
        void build_console_area();
        void build_console_title();
        wxPanel* build_console_title_panel();
        void build_console_title_label(wxPanel* panel);
        void render_console_title(wxPanel* panel);
        void build_console_body();
        wxPanel* build_console_body_panel();
        void build_console_list(wxPanel* panel);
        void render_console_body(wxPanel* panel);
        void build_send_area();
        void render_body();
        wxPanel* build_send_area_panel();
        void build_input_message(wxPanel* panel);
        void build_send_message_button(wxPanel* panel);
        void render_send_area(wxPanel* panel);
        void build_menu_bar();
        void build_file_drop_down_menu(wxMenuBar* menuBar);
        void build_actions_drop_down_menu(wxMenuBar* menuBar);
        void build_help_drop_down_menu(wxMenuBar* menuBar);
        void render_menu_bar(wxMenuBar* menuBar);
        void build_quit_menu_item(wxMenu* menu);
        void build_actions_menu_item(wxMenu* menu);
        void build_follow_menu_item(wxMenu* menu);
        void build_follow_user_dialog();
        void build_about_menu_item(wxMenu* menu);
        void display_console_log();
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnSend(wxCommandEvent& event);
        void on_close();
        void onCloseHandler(wxCloseEvent& event);
        void OnFollow(wxCloseEvent& event);
        void OnKeyDown(wxKeyEvent& event);
        std::string wx_to_str(wxString str);

        DECLARE_EVENT_TABLE()
    };
}
