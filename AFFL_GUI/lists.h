#ifndef LISTS_H_INCLUDED
#define LISTS_H_INCLUDED

#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/textfile.h>
#include <wx/grid.h>
#include <memory>

namespace
{
    struct ProcInfo
    {
        wxString id;
        wxString name;
    };
}

class BlackList
{
    private:
        std::unique_ptr<wxListBox> m_black_list;

    public:
        BlackList(wxListBox *black_list) : m_black_list(black_list) {}

        void addByPath(const wxString &path);
        void delSelected();
};

class ProcList
{
    private:
        wxTextFile file;
        std::unique_ptr<wxGrid> m_proc_list;
        std::unique_ptr<BlackList> m_black_list;

    public:
        ProcList(wxGrid *proc_list, BlackList *black_list)
            : m_proc_list(proc_list), m_black_list(black_list) {}

        void killSelected();
        void addSelectedToBlacklist();

        void update();

    private:
        ProcInfo getProcInfo(wxDir &dir, wxString &this_name);
        ProcInfo getSelectedProcInfo();
        void addProcInfoLine(const ProcInfo &pi);
        void delSelectedProcInfoLine();
};


#endif // LISTS_H_INCLUDED
