#ifndef WXSSTDMANAGER_H
#define WXSSTDMANAGER_H

#include "../widget.h"


enum std_widgets
{
    wxsNoneId = 0,
    
    /* Sizers */
    wxsGridSizerId,
    
    /* Controls */
    wxsButtonId,
    wxsToggleButtonId,      /* Warning - not compatible with XRC 2.4 */
    wxsCheckBoxId,
    wxsStaticTextId,
    
    /* Windows */
    wxsDialogId,
    wxsFrameId,
    wxsPanelId,
    
    /* Count */
    wxsStdIdCount
};

class wxsStdManagerT : public wxsWidgetManager
{
	public:
		wxsStdManagerT();
		virtual ~wxsStdManagerT();
		
		/** Getting number of handled widgets */
        virtual int GetCount(); 
        
        /** Getting widget's info */
        virtual const wxsWidgetInfo* GetWidgetInfo(int Number);
        
        /** Getting new widget */
        virtual wxsWidget* ProduceWidget(int Id);
        
        /** Killing widget */
        virtual void KillWidget(wxsWidget* Widget);
};

extern wxsStdManagerT wxsStdManager;

#endif // WXSSTDMANAGER_H
