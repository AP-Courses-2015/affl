/***************************************************************
 * Name:      AFFL_GUIApp.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2015-11-05
 * Copyright:  ()
 * License:
 **************************************************************/

#include "AFFL_GUIApp.h"

//(*AppHeaders
#include "AFFL_GUIMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(AFFL_GUIApp);

bool AFFL_GUIApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	AFFL_GUIFrame* Frame = new AFFL_GUIFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)

    return wxsOK;
}
