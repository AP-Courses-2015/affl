/***************************************************************
 * Name:      AFFL_GUIMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2015-11-05
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef AFFL_GUIMAIN_H
#define AFFL_GUIMAIN_H

//(*Headers(AFFL_GUIFrame)
#include <wx/grid.h>
#include <wx/sizer.h>
#include <wx/notebook.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/frame.h>
//*)

class AFFL_GUIFrame: public wxFrame
{
    public:

        AFFL_GUIFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~AFFL_GUIFrame();

    private:

        //(*Handlers(AFFL_GUIFrame)
        //*)

        //(*Identifiers(AFFL_GUIFrame)
        static const long ID_GRID1;
        static const long ID_ADD;
        static const long ID_KILL;
        static const long ID_PROC_LIST;
        static const long ID_BLACKLIST;
        static const long ID_NOTEBOOK1;
        //*)

        //(*Declarations(AFFL_GUIFrame)
        wxPanel* pnlBlacklist;
        wxGrid* grdProcList;
        wxButton* btnKill;
        wxNotebook* Notebook1;
        wxButton* btnAdd;
        wxPanel* pnlProcList;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // AFFL_GUIMAIN_H
