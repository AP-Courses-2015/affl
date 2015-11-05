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
const long AFFL_GUIFrame::ID_PANEL1 = wxNewId();
const long AFFL_GUIFrame::ID_PANEL2 = wxNewId();
const long AFFL_GUIFrame::ID_NOTEBOOK1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(AFFL_GUIFrame,wxFrame)
    //(*EventTable(AFFL_GUIFrame)
    //*)
END_EVENT_TABLE()

AFFL_GUIFrame::AFFL_GUIFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(AFFL_GUIFrame)
    wxBoxSizer* BoxSizer1;

    Create(parent, id, _("Application firewall"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    Notebook1 = new wxNotebook(this, ID_NOTEBOOK1, wxDefaultPosition, wxSize(467,533), 0, _T("ID_NOTEBOOK1"));
    Panel1 = new wxPanel(Notebook1, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    Panel2 = new wxPanel(Notebook1, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    Notebook1->AddPage(Panel1, _("Process list"), false);
    Notebook1->AddPage(Panel2, _("Blacklist"), false);
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
