/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include <sdk.h>

#ifndef CB_PRECOMP
    #include <wx/dataobj.h>
    #include <wx/intl.h>
    #include <wx/utils.h>
    #include <wx/sizer.h>
    #include <wx/settings.h>

    #include <manager.h>
    #include <logmanager.h>
    #include <projectmanager.h>
    #include <templatemanager.h>
    #include <pluginmanager.h>
    #include <editormanager.h>
    #include <configmanager.h>
#endif

#include "startherepage.h"
#include "main.h"
#include "appglobals.h"

#include <wx/clipbrd.h>
#include <wx/docview.h>
#include <wx/wxhtml.h>

const wxString g_StartHereTitle = _("Start here");
int idWin = wxNewId();

class MyHtmlWin : public wxHtmlWindow
{
    public:
        MyHtmlWin(StartHerePage* parent, int id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxHW_SCROLLBAR_AUTO)
            : wxHtmlWindow(parent, id, pos, size, style),
            m_pOwner(parent)
        {
        }

        void OnLinkClicked(const wxHtmlLinkInfo& link)
        {
            if (m_pOwner)
            {
                if (!m_pOwner->LinkClicked(link))
                    wxLaunchDefaultBrowser(link.GetHref());
            }
        }
    private:
        StartHerePage* m_pOwner;
};

BEGIN_EVENT_TABLE(StartHerePage, EditorBase)
END_EVENT_TABLE()

namespace
{

void ReplaceRecentProjectFiles(wxString &buf, const wxFileHistory &projects, const wxFileHistory &files)
{
    // replace history vars
    wxString links;

    links << _T("<table>\n<tr><td colspan=\"2\"><b>");
    links << _("Recent projects");
    links << _T("</b></td></tr>\n");
    if (projects.GetCount())
    {
        for (size_t i = 0; i < projects.GetCount(); ++i)
        {
            links << _T("<tr><td width=\"50\"><img alt=\"\" width=\"20\" src=\"blank.png\" />");
            links << wxString::Format(_T("<a href=\"CB_CMD_DELETE_HISTORY_PROJECT_%lu\"><img alt=\"\" src=\"trash_16x16.png\" /></a>"),
                                      static_cast<unsigned long>(i + 1));
            links << _T("<img alt=\"\"  width=\"10\" src=\"blank.png\" /></td><td width=\"10\">");
            links << wxString::Format(_T("<a href=\"CB_CMD_OPEN_HISTORY_PROJECT_%lu\">%s</a>"),
                                      static_cast<unsigned long>(i + 1), projects.GetHistoryFile(i).wx_str());
            links << _T("</td></tr>\n");
        }
    }
    else
    {
        links << _T("<tr><td style=\"width:2em;\"></td><td>&nbsp;&nbsp;&nbsp;&nbsp;");
        links << _("No recent projects");
        links << _T("</td></tr>\n");
    }

    links << _T("</table>\n<table>\n<tr><td colspan=\"2\"><b>");
    links << _("Recent files");
    links <<_T("</b></td></tr>\n");
    if (files.GetCount())
    {
        for (size_t i = 0; i < files.GetCount(); ++i)
        {
            links << _T("<tr><td width=\"50\"><img alt=\"\" width=\"20\" src=\"blank.png\" />");
            links << wxString::Format(_T("<a href=\"CB_CMD_DELETE_HISTORY_FILE_%lu\"><img alt=\"\" src=\"trash_16x16.png\" /></a>"),
                                      static_cast<unsigned long>(i + 1));
            links << _T("<img alt=\"\"  width=\"10\" src=\"blank.png\" /></td><td width=\"10\">");
            links << wxString::Format(_T("<a href=\"CB_CMD_OPEN_HISTORY_FILE_%lu\">%s</a>"),
                                      static_cast<unsigned long>(i + 1), files.GetHistoryFile(i).wx_str());
            links << _T("</td></tr>\n");
        }
    }
    else
    {
        links << _T("<tr><td style=\"width:2em;\"></td><td>&nbsp;&nbsp;&nbsp;&nbsp;");
        links << _("No recent files");
        links << _T("</td></tr>\n");
    }

    links << _T("</table>\n");


    // update page
    buf.Replace(_T("CB_VAR_RECENT_FILES_AND_PROJECTS"), links);
    buf.Replace(_T("CB_TXT_NEW_PROJECT"), _("Create a new project"));
    buf.Replace(_T("CB_TXT_OPEN_PROJECT"), _("Open an existing project"));
    buf.Replace(_T("CB_TXT_VISIT_FORUMS"), _("Visit the Code::Blocks forums"));
    buf.Replace(_T("CB_TXT_REPORT_BUG"), _("Report a bug"));
    buf.Replace(_T("CB_TXT_REQ_NEW_FEATURE"), _("Request a new feature"));
    buf.Replace(_T("CB_TXT_TIP_OF_THE_DAY"), _("Tip of the Day"));
}

} // anonymous namespace

StartHerePage::StartHerePage(wxEvtHandler* owner, const wxFileHistory *projects,
                             const wxFileHistory *files, wxWindow* parent)
    : EditorBase(parent, g_StartHereTitle),
    m_pOwner(owner),
    m_projects(projects),
    m_files(files)
{
    //ctor
    wxBoxSizer* bs = new wxBoxSizer(wxVERTICAL);

    wxString resPath = ConfigManager::ReadDataPath();

    // avoid gtk-critical because of sizes less than -1 (can happen with wxAuiNotebook/cbAuiNotebook)
    wxSize size = GetSize();
    size.x = std::max(size.x, -1);
    size.y = std::max(size.y, -1);

    m_pWin = new MyHtmlWin(this, idWin, wxPoint(0,0), size);

    // set default font sizes based on system default font size

    /* NOTE (mandrav#1#): wxWidgets documentation on wxHtmlWindow::SetFonts(),
    states that the sizes array accepts values from -2 to +4.
    My tests (under linux at least) have showed that it actually
    expects real point sizes. */

    wxFont systemFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    int sizes[7] = {};
    for (int i = 0; i < 7; ++i)
        sizes[i] = systemFont.GetPointSize();
    m_pWin->SetFonts(wxEmptyString, wxEmptyString, &sizes[0]);

    // must load the page this way because if we don't the image can't be found...
    m_pWin->LoadPage(resPath + _T("/start_here.zip#zip:start_here.html"));

    // alternate way to read the file so we can perform some search and replace
    // the C::B image referenced in the default start page can be found now
    // because we used LoadPage() above...
    wxString buf;
    wxFileSystem* fs = new wxFileSystem;
    wxFSFile* f = fs->OpenFile(resPath + _T("/start_here.zip#zip:start_here.html"));
    if (f)
    {
        wxInputStream* is = f->GetStream();
        char tmp[1024] = {};
        while (!is->Eof() && is->CanRead())
        {
            memset(tmp, 0, sizeof(tmp));
            is->Read(tmp, sizeof(tmp) - 1);
            buf << cbC2U((const char*)tmp);
        }
        delete f;
    }
    else
        buf = _("<html><body><h1>Welcome to Code::Blocks!</h1><br>The default start page seems to be missing...</body></html>");
    delete fs;

    #if defined(_LP64) || defined(_WIN64)
    const int bit_type = 64;
    #else
    const int bit_type = 32;
    #endif

    #ifdef __GNUC__
    revInfo.Printf(_T("%s (%s)   gcc %d.%d.%d %s/%s - %d bit"),
                    appglobals::AppActualVersionVerb.c_str(), ConfigManager::GetSvnDate().c_str(),
                    __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__, appglobals::AppPlatform.c_str(), appglobals::AppWXAnsiUnicode.c_str(), bit_type);
    #else
    revInfo.Printf(_T("%s (%s)   %s/%s"),
                    appglobals::AppActualVersionVerb.c_str(), ConfigManager::GetSvnDate().c_str(),
                    appglobals::AppPlatform.c_str(), appglobals::AppWXAnsiUnicode.c_str());
    #endif
    // perform var substitution
    buf.Replace(_T("CB_VAR_REVISION_INFO"), revInfo);
    buf.Replace(_T("CB_VAR_VERSION_VERB"), appglobals::AppActualVersionVerb);
    buf.Replace(_T("CB_VAR_VERSION"), appglobals::AppActualVersion);
    buf.Replace(_T("CB_SAFE_MODE"), PluginManager::GetSafeMode() ? _("SAFE MODE") : _T(""));

    m_OriginalPageContent = buf; // keep a copy of original for Reload()
    Reload();

    bs->Add(m_pWin, 1, wxEXPAND);
    SetSizer(bs);
    SetAutoLayout(true);
}

StartHerePage::~StartHerePage()
{
    //dtor
    //m_pWin->Destroy();
}

void StartHerePage::Reload()
{
    // Called every time something in the history changes.
    wxString buf = m_OriginalPageContent;
    ReplaceRecentProjectFiles(buf, *m_projects, *m_files);
    m_pWin->SetPage(buf);
}

bool StartHerePage::LinkClicked(const wxHtmlLinkInfo& link)
{
    //If it's already loading something, stop here
    if (Manager::Get()->GetProjectManager()->IsLoading())
        return true;
    if (!m_pOwner)
        return true;

    wxString href = link.GetHref();
    if (href.StartsWith(_T("CB_CMD_")))
    {
        wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, idStartHerePageLink);
        evt.SetString(link.GetHref());
        wxPostEvent(m_pOwner, evt);
        return true;
    }

    if (   href.IsSameAs(_T("http://www.codeblocks.org/"))
        || href.StartsWith(_T("http://developer.berlios.de/bugs/")) )
    {
        wxTextDataObject *data = new wxTextDataObject(revInfo);
        if (wxTheClipboard->Open())
        {
            wxTheClipboard->SetData(data);
            wxTheClipboard->Close();
        }
    }

    if(href.IsSameAs(_T("rev")))
    {
        wxTextDataObject *data = new wxTextDataObject(revInfo);
        if (wxTheClipboard->Open())
        {
            wxTheClipboard->SetData(data);
            wxTheClipboard->Close();
        }
        return true;
    }

    return false;
}
