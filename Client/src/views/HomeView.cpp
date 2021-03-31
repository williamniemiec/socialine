#include <iostream>
#include <vector>
#include <list>
#include <cstdlib>
#include <wx/msgdlg.h>
#include <wx/string.h>
#include <wx/intl.h>
#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/image.h>
#include <sstream>
#include "../../include/wx_pch.h"
#include "../../../Utils/StringUtils.hpp"
#include "../../include/views/HomeView.hpp"
#include "../../include/services/ClientCommunicationManager.h"
#include "../../include/models/manager/ClientNotificationManager.h"
#include "../../include/models/manager/ClientConsoleManager.hpp"

using namespace views;
using namespace controllers;
using namespace models;

//-------------------------------------------------------------------------
//      Attributes
//-------------------------------------------------------------------------
const std::string HomeView::TITLE = "SociaLine";
const int HomeView::WIDTH = 700;
const int HomeView::HEIGHT = 500;
const int HomeView::TITLE_HEIGHT = 20;
const int HomeView::HEADER_HEIGHT = 80;
const int HomeView::NOTIFICATIONS_AREA_HEIGHT = 140;
const int HomeView::SEND_BTN_WIDTH = 75;
const int HomeView::INPUT_WIDTH = WIDTH - SEND_BTN_WIDTH - 10;
const int HomeView::CONSOLE_AREA_HEIGHT = 140;
const long HomeView::ID_HEADER_IMG = wxNewId();
const long HomeView::ID_PNL_HEADER = wxNewId();
const long HomeView::ID_LBL_NOTIFICATIONS = wxNewId();
const long HomeView::ID_NOTIFICATIONS_TITLE = wxNewId();
const long HomeView::ID_NOTIFICATIONS_LIST = wxNewId();
const long HomeView::ID_PNL_NOTIFICATIONS_BODY = wxNewId();
const long HomeView::ID_LBL_CONSOLE = wxNewId();
const long HomeView::ID_PNL_CONSOLE_TITLE = wxNewId();
const long HomeView::ID_CONSOLE_LOG_LIST = wxNewId();
const long HomeView::ID_PNL_CONSOLE_BODY = wxNewId();
const long HomeView::ID_INPUT_MESSAGE = wxNewId();
const long HomeView::ID_BTN_SEND = wxNewId();
const long HomeView::ID_PNL_FOOTER = wxNewId();
const long HomeView::ID_PNL_MAIN = wxNewId();
const long HomeView::ID_MENU_QUIT = wxNewId();
const long HomeView::ID_MENU_ABOUT = wxNewId();
const long HomeView::ID_MENU_FOLLOW = wxNewId();
const long HomeView::ID_IN_FOLLOW_USERNAME = wxNewId();

BEGIN_EVENT_TABLE(HomeView,wxFrame)
    //(*EventTable(HomeView)
    //*)
END_EVENT_TABLE()


//-------------------------------------------------------------------------
//      Constructor & Destructors
//-------------------------------------------------------------------------
HomeView::HomeView(HomeController* homeController, User* user, wxWindow* parent, wxWindowID id)
{
    this->homeController = homeController;
    this->user = user;
    this->parent = parent;

    Bind(wxEVT_CHAR_HOOK, &HomeView::OnKeyDown, this);
}

HomeView::~HomeView()
{
}


//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
void HomeView::update(IObservable* observable, std::list<std::string> data)
{
    if (dynamic_cast<manager::ClientNotificationManager*>(observable) != nullptr)
    {
        for (std::string message : data)
        {
            notificationsList->Append(message);
            notificationsList->SetSelection(notificationsList->GetCount()-1);
        }
    }
    else if (dynamic_cast<manager::ClientConsoleManager*>(observable) != nullptr)
    {
        for (std::string message : data)
        {
            consoleList->Append(message);
            consoleList->SetSelection(consoleList->GetCount()-1);
        }
    }
}

void HomeView::render()
{
    buildWindow();
    buildIcon();
    buildHeader();
    buildBody();
    buildMenuBar();
    onClose();
}

void HomeView::buildWindow()
{
    Create(
            parent,
            wxID_ANY,
            _(TITLE),
            wxDefaultPosition,
            wxDefaultSize,
            wxDEFAULT_FRAME_STYLE,
            _T("wxID_ANY")
    );

    SetClientSize(wxSize(WIDTH,HEIGHT));
    SetMinSize(wxSize(WIDTH,HEIGHT));
    SetMaxSize(wxSize(WIDTH,HEIGHT));
}

void HomeView::buildIcon()
{
    wxIcon FrameIcon;
    FrameIcon.CopyFromBitmap(wxBitmap(wxImage(_T("src/assets/icon/socialine-icon.ico"))));
    SetIcon(FrameIcon);
}

void HomeView::buildHeader()
{
    wxPanel* pnlHeader = buildHeaderPanel();
    buildHeaderBackground(pnlHeader);
}

void HomeView::buildHeaderBackground(wxPanel* panel)
{
    new wxStaticBitmap(
            panel,
            ID_HEADER_IMG,
            wxBitmap(buildHeaderImage()),
            wxPoint(0,0),
            wxSize(WIDTH,HEADER_HEIGHT),
            wxSIMPLE_BORDER,
            _T("ID_HEADER_IMG")
    );
}

wxPanel* HomeView::buildHeaderPanel()
{
    return new wxPanel(
            this,
            ID_PNL_HEADER,
            wxDefaultPosition,
            wxSize(WIDTH, HEADER_HEIGHT),
            wxTAB_TRAVERSAL,
            _T("ID_PNL_HEADER")
    );
}

wxBitmap HomeView::buildHeaderImage()
{
    return wxImage(_T("src/assets/header/socialine-header-700x80.png")).Rescale(
            wxSize(WIDTH, HEADER_HEIGHT).GetWidth(),
            wxSize(WIDTH, HEADER_HEIGHT).GetHeight()
    );
}

void HomeView::buildBody()
{
    buildBodyPanel();
    buildNotificationsArea();
    buildConsoleArea();
    buildSendArea();
    renderBody();
}

void HomeView::buildBodyPanel()
{
    pnlMain = new wxPanel(
            this,
            ID_PNL_MAIN,
            wxPoint(0,HEADER_HEIGHT),
            wxSize(WIDTH,HEIGHT - HEADER_HEIGHT),
            wxTAB_TRAVERSAL,
            _T("ID_PNL_MAIN")
    );

    flexGridSizerMain = new wxFlexGridSizer(5, 2, 0, 0);
}

void HomeView::buildNotificationsArea()
{
    buildNotificationsTitle();
    buildNotificationsBody();
}

void HomeView::buildNotificationsTitle()
{
    wxPanel* pnlNotificationsTitle = buildNotificationsTitlePanel();
    buildNotificationsLabel(pnlNotificationsTitle);
    renderNotificationsTitle(pnlNotificationsTitle);
}

void HomeView::buildNotificationsLabel(wxPanel* panel)
{
    new wxStaticText(
            panel,
            ID_LBL_NOTIFICATIONS,
            _("Notifications"),
            wxPoint(8,0),
            wxDefaultSize,
            0,
            _T("ID_LBL_NOTIFICATIONS")
     );
}

wxPanel* HomeView::buildNotificationsTitlePanel()
{
    return new wxPanel(
            pnlMain,
            ID_NOTIFICATIONS_TITLE,
            wxDefaultPosition,
            wxSize(WIDTH-8,TITLE_HEIGHT),
            wxTAB_TRAVERSAL,
            _T("ID_NOTIFICATIONS_TITLE")
    );
}

void HomeView::renderNotificationsTitle(wxPanel* panel)
{
    flexGridSizerMain->Add(
            panel,
            1,
            wxALL|wxEXPAND,
            5
    );

    flexGridSizerMain->Add(
            -1,
            -1,
            1,
            wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL,
            5
    );
}

void HomeView::buildNotificationsBody()
{
    wxPanel* pnlNotificationsBody = buildNotificationsBodyPanel();
    buildNotificationsList(pnlNotificationsBody);
    renderNotificationsBody(pnlNotificationsBody);
}

wxPanel* HomeView::buildNotificationsBodyPanel()
{
    return new wxPanel(
            pnlMain,
            ID_PNL_NOTIFICATIONS_BODY,
            wxDefaultPosition,
            wxSize(WIDTH-8,NOTIFICATIONS_AREA_HEIGHT),
            wxTAB_TRAVERSAL,
            _T("ID_PNL_NOTIFICATIONS_BODY")
    );
}

void HomeView::buildNotificationsList(wxPanel* panel)
{
    notificationsList = new wxListBox(
            panel,
            ID_NOTIFICATIONS_LIST,
            wxDefaultPosition,
            wxSize(WIDTH-8,NOTIFICATIONS_AREA_HEIGHT),
            0,
            0,
            0,
            wxDefaultValidator,
            _T("ID_NOTIFICATIONS_LIST")
    );
}

void HomeView::renderNotificationsBody(wxPanel* panel)
{
    flexGridSizerMain->Add(
            panel,
            1,
            wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL,
            5
    );

    flexGridSizerMain->Add(
            -1,
            -1,
            1,
            wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL,
            5
    );
}

void HomeView::buildConsoleArea()
{
    buildConsoleTitle();
    buildConsoleBody();
}

void HomeView::buildConsoleTitle()
{
    wxPanel* pnlConsoleTitlePanel = buildConsoleTitlePanel();
    buildConsoleTitleLabel(pnlConsoleTitlePanel);
    renderConsoleTitle(pnlConsoleTitlePanel);
}

wxPanel* HomeView::buildConsoleTitlePanel()
{
    return new wxPanel(
            pnlMain,
            ID_PNL_CONSOLE_TITLE,
            wxDefaultPosition,
            wxSize(WIDTH-8,TITLE_HEIGHT),
            wxTAB_TRAVERSAL,
            _T("ID_PNL_CONSOLE_TITLE")
    );
}

void HomeView::buildConsoleTitleLabel(wxPanel* panel)
{
    new wxStaticText(
            panel,
            ID_LBL_CONSOLE,
            _("Console"),
            wxPoint(8,0),
            wxDefaultSize,
            0,
            _T("ID_LBL_CONSOLE")
     );
}

void HomeView::renderConsoleTitle(wxPanel* panel)
{
    flexGridSizerMain->Add(
            panel,
            1,
            wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL,
            5
    );

    flexGridSizerMain->Add(
            -1,
            -1,
            1,
            wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL,
            5
    );
}

void HomeView::buildConsoleBody()
{
    wxPanel* pnlConsoleBody = buildConsoleBodyPanel();
    buildConsoleList(pnlConsoleBody);
    renderConsoleBody(pnlConsoleBody);
}

wxPanel* HomeView::buildConsoleBodyPanel()
{
    return new wxPanel(
            pnlMain,
            ID_PNL_CONSOLE_BODY,
            wxDefaultPosition,
            wxSize(WIDTH-8,CONSOLE_AREA_HEIGHT),
            wxTAB_TRAVERSAL,
            _T("ID_PNL_CONSOLE_BODY")
    );
}

void HomeView::buildConsoleList(wxPanel* panel)
{
    consoleList = new wxListBox(
            panel,
            ID_CONSOLE_LOG_LIST,
            wxDefaultPosition,
            wxSize(WIDTH-8,CONSOLE_AREA_HEIGHT),
            0,
            0,
            0,
            wxDefaultValidator,
            _T("ID_CONSOLE_LOG_LIST")
    );
}

void HomeView::renderConsoleBody(wxPanel* panel)
{
    flexGridSizerMain->Add(
            panel,
            1,
            wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL,
            5
    );

    flexGridSizerMain->Add(
            -1,
            -1,
            1,
            wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL,
            5
    );
}

void HomeView::buildSendArea()
{
    wxPanel* pnlSendArea = buildSendAreaPanel();
    buildInputMessage(pnlSendArea);
    buildSendMessageButton(pnlSendArea);

    renderSendArea(pnlSendArea);
}

wxPanel* HomeView::buildSendAreaPanel()
{
    return new wxPanel(
            pnlMain,
            ID_PNL_FOOTER,
            wxDefaultPosition,
            wxSize(WIDTH-8,35),
            wxTAB_TRAVERSAL,
            _T("ID_PNL_FOOTER")
    );
}

void HomeView::buildInputMessage(wxPanel* panel)
{
    inMessage = new wxTextCtrl(
            panel,
            ID_INPUT_MESSAGE,
            wxEmptyString,
            wxPoint(0,0),
            wxSize(INPUT_WIDTH,30),
            0,
            wxDefaultValidator,
            _T("ID_INPUT_MESSAGE")
    );

    inMessage->SetHint("Message");
}

void HomeView::buildSendMessageButton(wxPanel* panel)
{
    new wxButton(
            panel,
            ID_BTN_SEND,
            _("Send"),
            wxPoint(INPUT_WIDTH + 2,0),
            wxSize(SEND_BTN_WIDTH,28),
            0,
            wxDefaultValidator,
            _T("ID_BTN_SEND")
     );

    Connect(
            ID_BTN_SEND,
            wxEVT_COMMAND_BUTTON_CLICKED,
            (wxObjectEventFunction)&HomeView::OnSend
    );
}

void HomeView::renderSendArea(wxPanel* panel)
{
    flexGridSizerMain->Add(
            panel,
            1,
            wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL,
            5
    );
}

void HomeView::renderBody()
{
    pnlMain->SetSizer(flexGridSizerMain);
    SetSizer(flexGridSizerMain);
    Layout();
}

void HomeView::buildMenuBar()
{
    wxMenuBar* menuBar = new wxMenuBar();

    buildFileDropDownMenu(menuBar);
    buildActionsDropDownMenu(menuBar);
    buildHelpDropDownMenu(menuBar);
    renderMenuBar(menuBar);
}

void HomeView::buildFileDropDownMenu(wxMenuBar* menuBar)
{
    wxMenu* fileMenu = new wxMenu();

    buildQuitMenuItem(fileMenu);

    menuBar->Append(fileMenu, _("&File"));
}

void HomeView::buildQuitMenuItem(wxMenu* menu)
{
    wxMenuItem* quitMenuItem = new wxMenuItem(
            menu,
            ID_MENU_QUIT,
            _("Quit\tAlt-F4"),
            _("Quit the application"),
            wxITEM_NORMAL
    );

    Connect(
            ID_MENU_QUIT,
            wxEVT_COMMAND_MENU_SELECTED,
            (wxObjectEventFunction)&HomeView::OnQuit
    );

    menu->Append(quitMenuItem);
}

void HomeView::buildActionsDropDownMenu(wxMenuBar* menuBar)
{
    wxMenu* actionsMenu = new wxMenu();

    buildActionsMenuItem(actionsMenu);

    menuBar->Append(actionsMenu, _("Actions"));
}

void HomeView::buildActionsMenuItem(wxMenu* menu)
{
    wxMenuItem* followMenuItem = new wxMenuItem(
            menu,
            ID_MENU_FOLLOW,
            _("Follow"),
            _("Follows an user"),
            wxITEM_NORMAL
    );

    buildFollowUserDialog();

    Connect(
            ID_MENU_FOLLOW,
            wxEVT_COMMAND_MENU_SELECTED,
            (wxObjectEventFunction)&HomeView::OnFollow
    );

    menu->Append(followMenuItem);
}

void HomeView::buildFollowUserDialog()
{
    dgFollowUsername = new wxTextEntryDialog(
            this,
            _("Username to be followed"),
            _("SociaLine - follow"),
            wxEmptyString,
            wxOK|wxCANCEL|wxCENTRE|wxWS_EX_VALIDATE_RECURSIVELY,
            wxDefaultPosition
    );
}

void HomeView::buildHelpDropDownMenu(wxMenuBar* menuBar)
{
    wxMenu* helpMenu = new wxMenu();

    buildAboutMenuItem(helpMenu);

    menuBar->Append(helpMenu, _("Help"));
}

void HomeView::buildAboutMenuItem(wxMenu* menu)
{
    wxMenuItem* aboutMenuItem = new wxMenuItem(
            menu,
            ID_MENU_ABOUT,
            _("About\tF1"),
            _("Show info about this application"),
            wxITEM_NORMAL
    );

    Connect(
            ID_MENU_ABOUT,
            wxEVT_COMMAND_MENU_SELECTED,
            (wxObjectEventFunction)&HomeView::OnAbout
    );

    menu->Append(aboutMenuItem);
}

void HomeView::renderMenuBar(wxMenuBar* menuBar)
{
    SetMenuBar(menuBar);
}

void HomeView::OnQuit(wxCommandEvent& event)
{
    Close();
}

void HomeView::OnAbout(wxCommandEvent& event)
{
    wxString msg = "Made by:\n\n"
        "-> Karine Farias\n"
        "-> Thiago Dal Moro\n"
        "-> Victor Melo\n"
        "-> William Niemiec\n\n"
        "http://github.com/williamniemiec/socialine \t";
    wxMessageBox(msg, _("SociaLine"));
}

void HomeView::OnSend(wxCommandEvent& event)
{
    homeController->sendMessage(wx_to_str(inMessage->GetValue()));

    inMessage->Clear();
    inMessage->SetFocus();
    pnlMain->SetFocus();
}

std::string HomeView::wx_to_str(wxString str)
{
    return std::string(str.mb_str(wxConvUTF8));
}

void HomeView::onClose()
{
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&HomeView::onCloseHandler);
}

void HomeView::onCloseHandler(wxCloseEvent& event)
{
    homeController->shutdownHandler();
}

void HomeView::OnFollow(wxCloseEvent& event)
{
    dgFollowUsername->ShowModal();
    homeController->follow(wx_to_str(dgFollowUsername->GetValue()));

    dgFollowUsername->SetValue("");
}

void HomeView::OnKeyDown(wxKeyEvent& event)
{
    if (event.ControlDown() && ((int)event.GetKeyCode() == 67 || (int)event.GetKeyCode() == 68)) {
        homeController->shutdownHandler();
    }
    event.Skip();
}
