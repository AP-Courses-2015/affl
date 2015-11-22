#include "lists.h"

void BlackList::init()
{
    if (m_phys_file.Exists())
        m_phys_file.Open();
    else
        m_phys_file.Create();

    if (!m_phys_file.IsOpened())
    {
        wxMessageBox(wxT("Can't open physical file"), wxT("Error"),
                     wxOK | wxCENTER | wxICON_ERROR);
        exit(-1);
    }

    if (!m_procfs_file.IsOpened())
    {
        wxMessageBox(wxT("Can't open blacklist file in procfs"), wxT("Error"),
                     wxOK | wxCENTER | wxICON_ERROR);
        //exit(-1);
    }

    wxString line;
    for (wxString line = m_phys_file.GetFirstLine();
         !m_phys_file.Eof();
         line = m_phys_file.GetNextLine())
    {
        m_black_list->AppendAndEnsureVisible(line);
        m_procfs_file.Write(line);
    }

    m_phys_file.Clear();
    for (size_t i = 0; i<m_black_list->GetCount(); i++)
        m_phys_file.AddLine(m_black_list->GetString(i));
    m_phys_file.Write();
}

void BlackList::addByPath(const wxString &path)
{
    char hash_value[33];
    const char *c_path = path.c_str().AsChar();
    if (makeHash(c_path, hash_value))
    {
        wxMessageBox(wxT("Can't read file"), wxT("Error"),
                     wxOK | wxCENTER | wxICON_ERROR);
    }

    if (m_black_list->FindString(hash_value) == wxNOT_FOUND)
    {
        m_black_list->AppendAndEnsureVisible(hash_value);
        m_phys_file.AddLine(hash_value);
        m_procfs_file.Write(hash_value);
        m_phys_file.Write();
    }
}

void BlackList::delSelected()
{
    if (m_black_list->GetSelection() == wxNOT_FOUND)
        return;
    m_procfs_file.Write(wxString::Format(wxT("k%i"), m_black_list->GetSelection()));
    m_phys_file.RemoveLine(m_black_list->GetSelection());
    m_black_list->removeSelected();
    m_phys_file.Write();
}

//=======================================================================================

void ProcList::killSelected()
{
    long id;
    if (m_proc_list->GetSelectedRows().Count() == 0)
        return;
    getSelectedProcInfo().id.ToLong(&id);

    wxProcess::Kill(id, wxSIGKILL);

    update();
}

void ProcList::addSelectedToBlacklist()
{
    if (m_proc_list->GetSelectedRows().Count() == 0)
        return;
    m_black_list->addByPath(getSelectedProcInfo().path);
    killSelected();
}

void ProcList::update()
{
    m_rows_using = 0;
    m_proc_list->ClearGrid();

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

    if (m_rows_count != m_rows_using)
    {
        m_proc_list->DeleteRows(m_rows_using, m_rows_count - m_rows_using);
        m_rows_count = m_rows_using;
    }

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
    if (m_rows_count == m_rows_using++)
    {
        m_proc_list->AppendRows();
        m_rows_count++;
    }

    m_proc_list->SetCellValue(m_rows_using-1, 0,
                              pi.id);
    m_proc_list->SetCellValue(m_rows_using-1, 1,
                              pi.name);
    m_proc_list->SetCellValue(m_rows_using-1, 2,
                              pi.path);
}
