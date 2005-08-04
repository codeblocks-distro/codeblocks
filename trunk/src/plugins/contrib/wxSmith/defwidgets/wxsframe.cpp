#include "wxsframe.h"

#include <wx/frame.h>
#include <wx/sizer.h>

#include "wxsstdmanager.h"
#include "../wxspropertiesman.h"

WXS_ST_BEGIN(wxsFrameStyles)

    WXS_ST(wxSTAY_ON_TOP)
    WXS_ST(wxCAPTION)
    WXS_ST(wxDEFAULT_DIALOG_STYLE)
    WXS_ST(wxDEFAULT_FRAME_STYLE)
    WXS_ST(wxTHICK_FRAME)
    WXS_ST(wxSYSTEM_MENU)
    WXS_ST(wxRESIZE_BORDER)
    WXS_ST(wxRESIZE_BOX)
    WXS_ST(wxCLOSE_BOX)

    WXS_ST(wxFRAME_NO_TASKBAR)
    WXS_ST(wxFRAME_SHAPED)
    WXS_ST(wxFRAME_TOOL_WINDOW)
    WXS_ST(wxFRAME_FLOAT_ON_PARENT)
    WXS_ST(wxMAXIMIZE_BOX)
    WXS_ST(wxMINIMIZE_BOX)
    WXS_ST(wxSTAY_ON_TOP)

    WXS_ST(wxNO_3D)
    WXS_ST(wxTAB_TRAVERSAL)
    WXS_ST(wxWS_EX_VALIDATE_RECURSIVELY)
    WXS_ST(wxFRAME_EX_METAL)

WXS_ST_END(wxsFrameStyles)

wxsFrame::wxsFrame(wxsWidgetManager* Man):
    wxsWindow(Man,propWindow),
    Centered(false)
{
}

wxsFrame::~wxsFrame()
{
}

const wxsWidgetInfo& wxsFrame::GetInfo()
{
    return *wxsStdManager.GetWidgetInfo(wxsFrameId);
}

void wxsFrame::CreateObjectProperties()
{
    wxsWidget::CreateObjectProperties();
	PropertiesObject.AddProperty("Title:",Title,0,false,false);
	PropertiesObject.AddProperty("Centered:",Centered,1,false,false);
}

