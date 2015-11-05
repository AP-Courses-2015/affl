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
#include <wx/listbox.h>
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
        static const long ID_PROC_LIST;
        static const long ID_ADD;
        static const long ID_KILL;
        static const long ID_PROC_PAN;
        static const long ID_BLACKLIST;
        static const long ID_ADD_PATH;
        static const long ID_DEL;
        static const long ID_BLACK_PAN;
        static const long ID_NOTEBOOK1;
        //*)

        //(*Declarations(AFFL_GUIFrame)
        wxPanel* pnlBlacklist;
        wxGrid* grdProcList;
        wxButton* btnAddPath;
        wxButton* btnKill;
        wxButton* btnDel;
        wxNotebook* Notebook1;
        wxListBox* lbBlacklist;
        wxButton* btnAdd;
        wxPanel* pnlProcList;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // AFFL_GUIMAIN_H
