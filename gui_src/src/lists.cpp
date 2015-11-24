#include "lists.h"

/*
    Проверяется наличие файлов с черным списком.

    Проверяются и заносятся в список заблокированные бинарники.
    Проверяется только наличие хэша.
    Валидность хэша не проверяется в связи с тем, что бинарник могли переименовать и
    новое имя файла может быть неизвестно.
    После этого файл очищается и все валидные записи заносятся обратно
*/
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
        exit(-1);
    }

    wxVector<wxString> values;
    wxString name;
    wxString hash_value;
    for (wxString line = m_phys_file.GetFirstLine();
         !m_phys_file.Eof();
         line = m_phys_file.GetNextLine())
    {
        hash_value = line.substr(0, 32);
        name = line.substr(32);
        if (name[0] != '/')
            continue;

        values.push_back(line);

        m_black_list->AppendAndEnsureVisible(name);

        m_procfs_file.Write(line);
    }

    m_phys_file.Clear();
    for (wxVector<wxString>::iterator it = values.begin(); it != values.end(); it++)
        m_phys_file.AddLine(*it);
    m_phys_file.Write();
}

void BlackList::addByPath(const wxString &path)
{
    if (!wxFileName::IsFileExecutable(path))
        return;

    char hash_value[33];
    if (makeHash(path.c_str().AsChar(), hash_value))
    {
        wxMessageBox(wxT("Can't read file"), wxT("Error"),
                     wxOK | wxCENTER | wxICON_ERROR);
    }

    wxString file_entry(hash_value + path);
    if (!IsHashExist(hash_value))
    {
        m_black_list->AppendAndEnsureVisible(path);
        m_phys_file.AddLine(file_entry);
        m_phys_file.Write();
        m_procfs_file.Write(file_entry);
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

bool BlackList::IsHashExist(const wxString &hash_value)
{
    for (wxString line = m_phys_file.GetFirstLine();
         !m_phys_file.Eof();
         line = m_phys_file.GetNextLine())
     {
        if (line.Find(hash_value) != wxNOT_FOUND)
            return true;
     }

     return false;
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

        ProcInfo pi = getProcInfo(next_dir);
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

ProcInfo ProcList::getProcInfo(wxString &pid)
{
    wxTextFile file;
    ProcInfo pi;
    wxDir dir("/proc");
    wxString full_dir_path(dir.GetNameWithSep() + pid);
    wxString full_file_name(full_dir_path + wxT("/comm"));

    pi.id = pid;
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
