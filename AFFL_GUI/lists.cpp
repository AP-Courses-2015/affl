#include "lists.h"
#include <memory>

void BlackList::addByPath(const wxString &path)
{
    if (m_black_list->FindString(path) == wxNOT_FOUND)
        m_black_list->AppendAndEnsureVisible(path);
}

void BlackList::delSelected()
{
    m_black_list->removeSelected();
}

//=======================================================================================

void ProcList::killSelected()
{
    long id;
    getSelectedProcInfo().id.ToLong(&id);

    kill(id, SIGKILL);

    update();
}

void ProcList::addSelectedToBlacklist()
{
    m_black_list->addByPath(getSelectedProcInfo().path);
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

    if (m_proc_list->GetRows() > 0)
        m_proc_list->DeleteRows(0, m_proc_list->GetRows());
    wxString file_name;
    wxTextFile file;
    do
    {
        long id;
        if (!next_dir.ToLong(&id))
            continue;

        ProcInfo pi = getProcInfo(dir, next_dir);
        //If we have path to binary file, then add to list
        if (!pi.path.IsEmpty())
            if (pi.path[0] == '/')
                addProcInfoLine(pi);
    } while(dir.GetNext(&next_dir));

    m_proc_list->AutoSizeColumns();
}

ProcInfo ProcList::getProcInfo(wxDir &dir, wxString &this_name)
{
    wxTextFile file;
    ProcInfo pi;
    wxString full_dir_path(dir.GetNameWithSep() + this_name);
    wxString full_file_name(full_dir_path + wxT("/comm"));

    pi.id = this_name;
    file.Open(full_file_name);
    pi.name = file.GetFirstLine();
    file.Close();
    full_file_name = full_dir_path + wxT("/cmdline");
    file.Open(full_file_name);
    pi.path = file.GetFirstLine().BeforeFirst(' ').BeforeFirst('\0');
    file.Close();

    return pi;
}

ProcInfo ProcList::getSelectedProcInfo()
{
    ProcInfo pi;
    int selected_row = m_proc_list->GetSelectedRows().Item(0);
    pi.id = m_proc_list->GetCellValue(selected_row, 0);
    pi.name = m_proc_list->GetCellValue(selected_row, 1);
    pi.path = m_proc_list->GetCellValue(selected_row, 2);

    return pi;
}

void ProcList::addProcInfoLine(const ProcInfo &pi)
{
    m_proc_list->AppendRows();
    m_proc_list->SetCellValue(m_proc_list->GetRows()-1, 0,
                              pi.id);
    m_proc_list->SetCellValue(m_proc_list->GetRows()-1, 1,
                              pi.name);
    m_proc_list->SetCellValue(m_proc_list->GetRows()-1, 2,
                              pi.path);
}
