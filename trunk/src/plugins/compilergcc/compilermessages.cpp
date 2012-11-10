/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include "sdk.h"
#ifndef CB_PRECOMP
    #include <wx/intl.h>
    #include <wx/listctrl.h>
    #include <wx/menu.h>
    #include <wx/string.h>
    #include <wx/utils.h>
    #include "globals.h"
    #include "manager.h"
#endif
#include <wx/clipbrd.h>
#include <wx/dataobj.h>
#include "compilererrors.h"
#include "compilermessages.h"
#include "prep.h"

namespace
{
    int idList = wxNewId();
};

BEGIN_EVENT_TABLE(CompilerMessages, wxEvtHandler)
END_EVENT_TABLE()

CompilerMessages::CompilerMessages(const wxArrayString& titles, const wxArrayInt& widths)
    : ListCtrlLogger(titles, widths, true)
{
    //ctor
}

CompilerMessages::~CompilerMessages()
{
	//dtor
	Disconnect(idList, -1, wxEVT_COMMAND_LIST_ITEM_SELECTED,
            (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)
            &CompilerMessages::OnClick);
    Disconnect(idList, -1, wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
            (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)
            &CompilerMessages::OnDoubleClick);
}

wxWindow* CompilerMessages::CreateControl(wxWindow* parent)
{
    ListCtrlLogger::CreateControl(parent);
    control->SetId(idList);
    Connect(idList, -1, wxEVT_COMMAND_LIST_ITEM_SELECTED,
            (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)
            &CompilerMessages::OnClick);
    Connect(idList, -1, wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
            (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)
            &CompilerMessages::OnDoubleClick);
    Manager::Get()->GetAppWindow()->PushEventHandler(this);
    return control;
}

void CompilerMessages::DestroyControls()
{
    if ( !Manager::Get()->IsAppShuttingDown() )
    {
        Manager::Get()->GetAppWindow()->RemoveEventHandler(this);
    }
}

void CompilerMessages::FocusError(int nr)
{
    control->SetItemState(nr, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED);
    control->EnsureVisible(nr);
}

void CompilerMessages::OnClick(cb_unused wxCommandEvent& event)
{
    // single and double-click, behave the same

    // a compiler message has been clicked
    // go to the relevant file/line
    if (control->GetSelectedItemCount() == 0 || !m_pErrors)
        return;

    // find selected item index
    int index = control->GetNextItem(-1,
                                     wxLIST_NEXT_ALL,
                                     wxLIST_STATE_SELECTED);

    // call the CompilerErrors* ptr; it 'll do all the hard work ;)
    m_pErrors->GotoError(index);
}

void CompilerMessages::OnDoubleClick(wxCommandEvent& event)
{
    // single and double-click, behave the same
    OnClick(event);
    return;
}
