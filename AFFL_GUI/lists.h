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

        wxString path;
    };
}

//Open protected method "DoDeleteOneItem()"
class OpenListBox : public wxListBox
{
    public:
        OpenListBox() : wxListBox() {}

        OpenListBox(wxWindow *parent, wxWindowID id,
                        const wxPoint& pos = wxDefaultPosition,
                        const wxSize& size = wxDefaultSize,
                        int n = 0, const wxString choices[] = (const wxString *) NULL,
                        long style = 0,
                        const wxValidator& validator = wxDefaultValidator,
                        const wxString& name = wxListBoxNameStr)
                : wxListBox(parent, id, pos, size, n, choices, style, validator, name) {}

        OpenListBox( wxWindow *parent, wxWindowID id,
                        const wxPoint& pos,
                        const wxSize& size,
                        const wxArrayString& choices,
                        long style = 0,
                        const wxValidator& validator = wxDefaultValidator,
                        const wxString& name = wxListBoxNameStr )
                : wxListBox(parent, id, pos, size, choices, style, validator, name) {}

    public:
        void removeSelected()
        {
            DoDeleteOneItem(GetSelection());
        }
};

class BlackList
{
    private:
        std::unique_ptr<OpenListBox>m_black_list;

    public:
        BlackList(OpenListBox *black_list) : m_black_list(black_list) {}

        void addByPath(const wxString &path);
        void delSelected();
};

class ProcList
{
    private:
        std::unique_ptr<wxGrid> m_proc_list;
        std::unique_ptr<BlackList> m_black_list;

    public:
        ProcList(wxGrid *proc_list, BlackList *black_list)
            : m_proc_list(proc_list), m_black_list(black_list)
        {
            m_proc_list->SetRowLabelSize(0);
            m_proc_list->SetSelectionMode(wxGrid::wxGridSelectRows);
            m_proc_list->DisableDragRowSize();
        }

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
