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
#include <wx/sizer.h>
#include <wx/notebook.h>
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
        static const long ID_PANEL1;
        static const long ID_PANEL2;
        static const long ID_NOTEBOOK1;
        //*)

        //(*Declarations(AFFL_GUIFrame)
        wxPanel* Panel1;
        wxPanel* Panel2;
        wxNotebook* Notebook1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // AFFL_GUIMAIN_H
