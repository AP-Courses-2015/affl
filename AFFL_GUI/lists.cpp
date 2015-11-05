#include "lists.h"

void BlackList::addByPath(const wxString &path)
{
    if (!m_black_list->FindString(path))
        m_black_list->AppendAndEnsureVisible(path);
}

void BlackList::delSelected()
{
    //m_black_list->DoDeleteOneItem(m_black_list->GetSelection());
}

//=======================================================================================

void ProcList::killSelected()
{
    wxString id = getSelectedProcInfo().id;

    wxShell(wxString::Format("kill %s", id));

    delSelectedProcInfoLine();
}

void ProcList::addSelectedToBlacklist()
{
    //wxString path = getSelectedProcInfo().path;
    //m_black_list->addByPath(path);
    killSelected();
}

void ProcList::update()
{
    //Template disable automatic errors
    wxLogNull log_null;

    wxDir dir("/proc");

    if (!dir.IsOpened())
    {
        wxMessageBox(wxT("Can't open /proc dir"), wxT("Error"), wxOK | wxCENTER | wxICON_ERROR);

        return;
    }


    wxString next_dir;
    int flags = wxDIR_DIRS;
    if (!dir.GetFirst(&next_dir, wxT(""), flags))
    {
        wxMessageBox(wxT("Can't find dirs in /proc dir"), wxT("Error"), wxOK | wxCENTER | wxICON_ERROR);

        return;
    }

//    m_proc_list->Clear();
    wxString file_name;
    wxTextFile file;
    do
    {
        long id;
        if (!next_dir.ToLong(&id))
            continue;

        ProcInfo pi = getProcInfo(dir, next_dir);
        addProcInfoLine(pi);
    } while(dir.GetNext(&next_dir));

    m_proc_list->AutoSizeColumns();
}

ProcInfo ProcList::getProcInfo(wxDir &dir, wxString &this_name)
{
    ProcInfo pi;

    pi.id = this_name;
    wxString tmp(dir.GetNameWithSep() + this_name + wxT("/cmdline"));
    file.Open(tmp);
    pi.name = file.GetFirstLine();
    file.Close();

    return pi;
}

ProcInfo ProcList::getSelectedProcInfo()
{
    ProcInfo pi;
    pi.id = wxT("0");
    pi.name = wxEmptyString;

    /*m_proc_list->
    wxGrid::GetCellValue()*/

    return pi;
}

void ProcList::addProcInfoLine(const ProcInfo &pi)
{
    m_proc_list->AppendRows();
    m_proc_list->SetCellValue(m_proc_list->GetRows()-1, 0,
                              pi.id);
    m_proc_list->SetCellValue(m_proc_list->GetRows()-1, 1,
                              pi.name);
}

void ProcList::delSelectedProcInfoLine()
{
    /*m_id_list->DoDeleteOneItem(GetSelection());
    m_names_list->DoDeleteOneItem(GetSelection());*/
}
