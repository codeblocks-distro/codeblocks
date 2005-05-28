#include "wxswindoweditor.h"

#include "widget.h"
#include <wx/settings.h>
#include <wx/scrolwin.h>
#include "wxspropertiesman.h"
#include "wxspalette.h"
#include "wxsmith.h"

wxsWindowEditor::wxsWindowEditor(wxWindow* parent, const wxString& title,wxsResource* Resource):
    wxsEditor(parent,title,Resource),
    CurrentWidget(NULL)
{
    wxSizer* Sizer = new wxBoxSizer(wxVERTICAL);

    Scroll = new wxScrolledWindow(this);
    Scroll->SetScrollRate(4,4);
    
    Sizer->Add(Scroll,1,wxGROW);
    Scroll->SetScrollRate(4,4);
    
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_APPWORKSPACE));
    Scroll->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_APPWORKSPACE));
    
    SetSizer(Sizer);
    SetAutoLayout(true);
    
    wxsEvent SelectEvent(wxEVT_SELECT_RES,0,Resource);
    wxPostEvent(wxSmith::Get(),SelectEvent);
}

wxsWindowEditor::~wxsWindowEditor()
{
	KillCurrentPreview();
}

static void WidgetRefreshReq(wxWindow* Wnd)
{
    if ( !Wnd ) return;
    Wnd->Refresh(true);
    
    wxWindowList& List = Wnd->GetChildren();
    for ( wxWindowListNode* Node = List.GetFirst(); Node; Node = Node->GetNext() )
    {
        wxWindow* Win = Node->GetData();
        WidgetRefreshReq(Win);
    }
}

void wxsWindowEditor::BuildPreview(wxsWidget* TopWidget)
{
    Scroll->SetSizer(NULL);
    Freeze();
    
    KillCurrentPreview();

    // Creating new sizer

    wxWindow* TopPreviewWindow = TopWidget ? TopWidget->CreatePreview(Scroll,this) : NULL;
    CurrentWidget = TopWidget;
    
    SetVirtualSizeHints(1,1);
    if ( TopPreviewWindow )
    {
        wxSizer* NewSizer = new wxGridSizer(1);
        NewSizer->Add(TopPreviewWindow,0,wxALIGN_CENTRE_VERTICAL|wxALIGN_CENTRE_HORIZONTAL|wxALL,10);
        Scroll->SetSizer(NewSizer);
        NewSizer->SetVirtualSizeHints(Scroll);
        TopPreviewWindow->Refresh();
    }
    
    Thaw();
    Layout();
    WidgetRefreshReq(this);
}

void wxsWindowEditor::KillCurrentPreview()
{
    if ( CurrentWidget ) CurrentWidget->KillPreview();
    CurrentWidget = NULL;
   
}

void wxsWindowEditor::OnMouseClick(wxMouseEvent& event)
{
    if ( CurrentWidget )
    {
        wxsPropertiesMan::Get()->SetActiveWidget(CurrentWidget);
    }
}

void wxsWindowEditor::OnActivate(wxActivateEvent& event)
{
    if ( event.GetActive() )
    {
        wxsEvent Select(wxEVT_SELECT_RES,0,GetResource());
        wxPostEvent(wxSmith::Get(),Select);
    }
}

void wxsWindowEditor::PreviewReshaped()
{
    SetSizer(NULL);
    
    if ( CurrentWidget && CurrentWidget->GetPreview() )
    {
        wxSizer* NewSizer = new wxGridSizer(1);
        NewSizer->Add(CurrentWidget->GetPreview(),0,wxALIGN_CENTRE_VERTICAL|wxALIGN_CENTRE_HORIZONTAL);
        SetSizer(NewSizer);
        Layout();
        CurrentWidget->GetPreview()->Refresh();
    }
}

void wxsWindowEditor::MyUnbind()
{
    wxsEvent Unselect(wxEVT_UNSELECT_RES,0,GetResource());
    wxPostEvent(wxSmith::Get(),Unselect);
    KillCurrentPreview();
}

BEGIN_EVENT_TABLE(wxsWindowEditor,wxsEditor)
    EVT_LEFT_DOWN(wxsWindowEditor::OnMouseClick)
    EVT_ACTIVATE(wxsWindowEditor::OnActivate)
END_EVENT_TABLE()
