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
#include <wx/filedlg.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <wx/timer.h>
//*)
#include "lists.h"

class AFFL_GUIFrame: public wxFrame
{
    public:
        AFFL_GUIFrame(wxWindow* parent,wxWindowID id = -1);

        virtual ~AFFL_GUIFrame();

    private:

        //(*Handlers(AFFL_GUIFrame)
        void OnbtnKillClick(wxCommandEvent& event);
        void OnbtnAddClick(wxCommandEvent& event);
        void OnbtnAddPathClick(wxCommandEvent& event);
        void OnbtnDelClick(wxCommandEvent& event);
        void OntRefreshTrigger(wxTimerEvent& event);
        void OnKillFocus(wxFocusEvent& event);
        void OnSetFocus(wxFocusEvent& event);
        void OnClose(wxCloseEvent& event);
        //*)
        void OngrdRangeSelect(wxGridRangeSelectEvent &event);

        //(*Identifiers(AFFL_GUIFrame)
        static const long ID_PROC_LIST;
        static const long ID_ADD;
        static const long ID_KILL;
        static const long ID_PROC_PAN;
        static const long ID_ADD_PATH;
        static const long ID_DEL;
        static const long ID_BLACK_PAN;
        static const long ID_NOTEBOOK1;
        static const long ID_REFRESH;
        //*)
        static const long ID_BLACKLIST;

        //(*Declarations(AFFL_GUIFrame)
        wxFileDialog* fdAddByPath;
        wxPanel* pnlBlacklist;
        wxTimer tRefresh;
        wxGrid* grdProcList;
        wxButton* btnAddPath;
        wxButton* btnKill;
        wxButton* btnDel;
        wxNotebook* Notebook1;
        wxButton* btnAdd;
        wxPanel* pnlProcList;
        //*)
        OpenListBox* lbBlacklist;

        ProcList *proc_list;
        BlackList *black_list;

        DECLARE_EVENT_TABLE()

    private:
        void initKernelModule();
};

#endif // AFFL_GUIMAIN_H
