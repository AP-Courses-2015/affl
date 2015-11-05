/***************************************************************
 * Name:      AFFL_GUIMain.cpp
 * Purpose:   Code for Application Frame
 * Author:     ()
 * Created:   2015-11-05
 * Copyright:  ()
 * License:
 **************************************************************/

#include "AFFL_GUIMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(AFFL_GUIFrame)
#include <wx/string.h>
#include <wx/intl.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(AFFL_GUIFrame)
const long AFFL_GUIFrame::ID_PROC_LIST = wxNewId();
const long AFFL_GUIFrame::ID_ADD = wxNewId();
const long AFFL_GUIFrame::ID_KILL = wxNewId();
const long AFFL_GUIFrame::ID_PROC_PAN = wxNewId();
const long AFFL_GUIFrame::ID_BLACKLIST = wxNewId();
const long AFFL_GUIFrame::ID_ADD_PATH = wxNewId();
const long AFFL_GUIFrame::ID_DEL = wxNewId();
const long AFFL_GUIFrame::ID_BLACK_PAN = wxNewId();
const long AFFL_GUIFrame::ID_NOTEBOOK1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(AFFL_GUIFrame,wxFrame)
    //(*EventTable(AFFL_GUIFrame)
    //*)
END_EVENT_TABLE()

AFFL_GUIFrame::AFFL_GUIFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(AFFL_GUIFrame)
    wxBoxSizer* BoxSizer3;
    wxBoxSizer* BoxSizer7;
    wxBoxSizer* BoxSizer2;
    wxBoxSizer* BoxSizer4;
    wxBoxSizer* BoxSizer1;
    wxBoxSizer* BoxSizer6;
    wxBoxSizer* BoxSizer5;

    Create(parent, id, _("Application firewall"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    Notebook1 = new wxNotebook(this, ID_NOTEBOOK1, wxDefaultPosition, wxSize(508,533), 0, _T("ID_NOTEBOOK1"));
    pnlProcList = new wxPanel(Notebook1, ID_PROC_PAN, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PROC_PAN"));
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    grdProcList = new wxGrid(pnlProcList, ID_PROC_LIST, wxDefaultPosition, wxSize(295,471), 0, _T("ID_PROC_LIST"));
    grdProcList->CreateGrid(0,2);
    grdProcList->EnableEditing(true);
    grdProcList->EnableGridLines(true);
    grdProcList->SetRowLabelSize(1);
    grdProcList->SetColLabelValue(0, _("ID"));
    grdProcList->SetColLabelValue(1, _("Name"));
    grdProcList->SetDefaultCellFont( grdProcList->GetFont() );
    grdProcList->SetDefaultCellTextColour( grdProcList->GetForegroundColour() );
    BoxSizer3->Add(grdProcList, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer2->Add(BoxSizer3, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer4 = new wxBoxSizer(wxVERTICAL);
    btnAdd = new wxButton(pnlProcList, ID_ADD, _("Add to blacklist"), wxDefaultPosition, wxSize(129,27), 0, wxDefaultValidator, _T("ID_ADD"));
    BoxSizer4->Add(btnAdd, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer4->Add(20,25,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    btnKill = new wxButton(pnlProcList, ID_KILL, _("Kill process"), wxDefaultPosition, wxSize(129,27), 0, wxDefaultValidator, _T("ID_KILL"));
    BoxSizer4->Add(btnKill, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer2->Add(BoxSizer4, 0, wxALL|wxALIGN_RIGHT|wxALIGN_TOP, 0);
    pnlProcList->SetSizer(BoxSizer2);
    BoxSizer2->Fit(pnlProcList);
    BoxSizer2->SetSizeHints(pnlProcList);
    pnlBlacklist = new wxPanel(Notebook1, ID_BLACK_PAN, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_BLACK_PAN"));
    BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    lbBlacklist = new wxListBox(pnlBlacklist, ID_BLACKLIST, wxDefaultPosition, wxSize(230,475), 0, 0, 0, wxDefaultValidator, _T("ID_BLACKLIST"));
    BoxSizer6->Add(lbBlacklist, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer5->Add(BoxSizer6, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer7 = new wxBoxSizer(wxVERTICAL);
    btnAddPath = new wxButton(pnlBlacklist, ID_ADD_PATH, _("Add by path"), wxDefaultPosition, wxSize(126,27), 0, wxDefaultValidator, _T("ID_ADD_PATH"));
    BoxSizer7->Add(btnAddPath, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer7->Add(20,26,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    btnDel = new wxButton(pnlBlacklist, ID_DEL, _("Delete "), wxDefaultPosition, wxSize(127,27), 0, wxDefaultValidator, _T("ID_DEL"));
    BoxSizer7->Add(btnDel, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer5->Add(BoxSizer7, 0, wxALL|wxALIGN_RIGHT|wxALIGN_TOP, 0);
    pnlBlacklist->SetSizer(BoxSizer5);
    BoxSizer5->Fit(pnlBlacklist);
    BoxSizer5->SetSizeHints(pnlBlacklist);
    Notebook1->AddPage(pnlProcList, _("Process list"), false);
    Notebook1->AddPage(pnlBlacklist, _("Blacklist"), false);
    BoxSizer1->Add(Notebook1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    SetSizer(BoxSizer1);
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);
    //*)
}

AFFL_GUIFrame::~AFFL_GUIFrame()
{
    //(*Destroy(AFFL_GUIFrame)
    //*)
}
