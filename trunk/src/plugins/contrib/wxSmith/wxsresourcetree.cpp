#include "wxsheaders.h"
#include "wxsresourcetree.h"
#include "wxsglobals.h"
#include "resources/wxswindowres.h"
#include "resources/wxswindowresdataobject.h"
#include "wxsevent.h"
#include "widget.h"
#include "wxswidgetfactory.h"
#include "wxswinundobuffer.h"

void wxsResourceTree::OnSelectResource(wxTreeEvent& event)
{
    wxsResourceTreeData* Data = ((wxsResourceTreeData*)GetItemData(event.GetItem()));
    if ( Data )
    {
        switch ( Data->Type )
        {
            case wxsResourceTreeData::tWidget:
                {
                    wxsSelectWidget(Data->Widget);
                }
                break;

            case wxsResourceTreeData::tResource:
                {
                    wxsSelectRes(Data->Resource);
                }
                break;

            default:;
        }
    }
}

void wxsResourceTree::OnBeginDrag(wxTreeEvent& event)
{
    wxsResourceTreeData* Data = ((wxsResourceTreeData*)GetItemData(event.GetItem()));
    if ( !Data ) return;
    if ( Data->Type == wxsResourceTreeData::tWidget &&
         Data->Widget->GetParent() )
    {
        Dragged = Data->Widget;
        event.Allow();
    }
}

void wxsResourceTree::OnEndDrag(wxTreeEvent& event)
{
    wxsResourceTreeData* Data = ((wxsResourceTreeData*)GetItemData(event.GetItem()));
    if ( !Data ) return;
    // Because we're dragging widgets, both widgets muse have open eeditors
    if ( Data->Type == wxsResourceTreeData::tWidget )
    {
        wxsWidget* Dest = Data->Widget;
        if ( Dest == Dragged ) return;

        // Determining insertion method
        bool InsertBefore = true;

        if (  !Dest->GetParent() ||
             ( Dest->GetInfo().Sizer &&
              !Dest->GetParent()->GetInfo().Sizer ) )
        {
            InsertBefore = false;
        }

        // Copying data
        wxsWindowResDataObject Data;
        Data.AddWidget(Dragged);

        wxsWidget* Insert = Data.BuildWidget(Dest->GetResource());
        if ( !Insert )
        {
        	DebLog(_("wxSmith: Error while cloning widget"));
        	return;
        }

        wxsWindowEditor* Editor1 = (wxsWindowEditor*)Dest->GetResource()->GetEditor();
        wxsWindowEditor* Editor2 = (wxsWindowEditor*)Dragged->GetResource()->GetEditor();
        Editor1->StartMultipleChange();
        bool Done;
        if ( InsertBefore )
        {
            Done = Editor1->InsertBefore(Insert,Dest);
        }
        else
        {
            Done = Editor1->InsertInto(Insert,Dest);
        }

        if ( Editor1 != Editor2 )
        {
        	Editor1->EndMultipleChange();
        	Editor2->StartMultipleChange();
        }

        if ( Done && !::wxGetKeyState(WXK_CONTROL) )
        {
        	wxsKILL(Dragged);
        }

        Dest->GetResource()->CheckBaseProperties(true,NULL);

        Editor2->EndMultipleChange();

        if ( Done )
        {
        	Editor1->GetUndoBuff()->StoreChange();
        }
    }
}

BEGIN_EVENT_TABLE(wxsResourceTree,wxTreeCtrl)
    EVT_TREE_SEL_CHANGED(wxID_ANY,wxsResourceTree::OnSelectResource)
    EVT_TREE_BEGIN_DRAG(wxID_ANY,wxsResourceTree::OnBeginDrag)
    EVT_TREE_BEGIN_RDRAG(wxID_ANY,wxsResourceTree::OnBeginDrag)
    EVT_TREE_END_DRAG(wxID_ANY,wxsResourceTree::OnEndDrag)
END_EVENT_TABLE()
