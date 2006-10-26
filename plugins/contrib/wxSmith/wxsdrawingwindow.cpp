#include "wxsdrawingwindow.h"

#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/dcclient.h>
#include <wx/dcscreen.h>
#include <wx/dcmemory.h>
#include <wx/dcbuffer.h>

#include <manager.h>
#include <messagemanager.h>

#ifdef FETCHING_SYSTEM2
    const int wxEVT_FETCH_SEQUENCE = wxNewEventType();
#endif


/** \brief Drawing panel
 *
 * This panel is put over all other items in wxsDrawingWindow class. It's
 * responsible for fetching paing, mouse and keyboard events
 */
class wxsDrawingWindow::DrawingPanel: public wxPanel
{
    public:

        /** \brief Ctor */
        DrawingPanel(wxsDrawingWindow* Parent): wxPanel(Parent,-1)
        {
            // Connecting event handlers of drawing window
            Connect(-1,wxEVT_PAINT,(wxObjectEventFunction)&wxsDrawingWindow::PanelPaint,NULL,Parent);
            Connect(-1,wxEVT_LEFT_DOWN,(wxObjectEventFunction)&wxsDrawingWindow::PanelMouse,NULL,Parent);
            Connect(-1,wxEVT_LEFT_UP,(wxObjectEventFunction)&wxsDrawingWindow::PanelMouse,NULL,Parent);
            Connect(-1,wxEVT_LEFT_DCLICK,(wxObjectEventFunction)&wxsDrawingWindow::PanelMouse,NULL,Parent);
            Connect(-1,wxEVT_MIDDLE_DOWN,(wxObjectEventFunction)&wxsDrawingWindow::PanelMouse,NULL,Parent);
            Connect(-1,wxEVT_MIDDLE_UP,(wxObjectEventFunction)&wxsDrawingWindow::PanelMouse,NULL,Parent);
            Connect(-1,wxEVT_MIDDLE_DCLICK,(wxObjectEventFunction)&wxsDrawingWindow::PanelMouse,NULL,Parent);
            Connect(-1,wxEVT_RIGHT_DOWN,(wxObjectEventFunction)&wxsDrawingWindow::PanelMouse,NULL,Parent);
            Connect(-1,wxEVT_RIGHT_UP,(wxObjectEventFunction)&wxsDrawingWindow::PanelMouse,NULL,Parent);
            Connect(-1,wxEVT_RIGHT_DCLICK,(wxObjectEventFunction)&wxsDrawingWindow::PanelMouse,NULL,Parent);
            Connect(-1,wxEVT_MOTION,(wxObjectEventFunction)&wxsDrawingWindow::PanelMouse,NULL,Parent);
            Connect(-1,wxEVT_ENTER_WINDOW,(wxObjectEventFunction)&wxsDrawingWindow::PanelMouse,NULL,Parent);
            Connect(-1,wxEVT_LEAVE_WINDOW,(wxObjectEventFunction)&wxsDrawingWindow::PanelMouse,NULL,Parent);
            Connect(-1,wxEVT_MOUSEWHEEL,(wxObjectEventFunction)&wxsDrawingWindow::PanelMouse,NULL,Parent);
            Connect(-1,wxEVT_KEY_DOWN,(wxObjectEventFunction)&wxsDrawingWindow::PanelKeyboard,NULL,Parent);
            Connect(-1,wxEVT_KEY_UP,(wxObjectEventFunction)&wxsDrawingWindow::PanelKeyboard,NULL,Parent);
            Connect(-1,wxEVT_CHAR,(wxObjectEventFunction)&wxsDrawingWindow::PanelKeyboard,NULL,Parent);
        }

};

BEGIN_EVENT_TABLE(wxsDrawingWindow,wxScrolledWindow)
END_EVENT_TABLE()

wxsDrawingWindow::wxsDrawingWindow(wxWindow* Parent,wxWindowID id):
    wxScrolledWindow(Parent,id),
    Panel(NULL),
    PaintAfterFetch(false),
    WaitTillHideChildren(false),
    IsBlockFetch(false),
    Bitmap(NULL)
    #ifdef FETCHING_SYSTEM2
    ,DuringFetch(false)
    #endif
{
    #ifdef FETCHING_SYSTEM2
    // Strange - it seems that by declaring this event in event table, it's not processed
    Connect(-1,-1,wxEVT_FETCH_SEQUENCE,(wxObjectEventFunction)&wxsDrawingWindow::OnFetchSequence);
    #endif
    ContentChanged();
    SetScrollbars(5,5,1,1,0,0,true);
}

wxsDrawingWindow::~wxsDrawingWindow()
{
    if ( Bitmap ) delete Bitmap;
}

void wxsDrawingWindow::ContentChanged()
{
    wxSize Size = GetVirtualSize();

    // Generating new bitmap
    if ( Bitmap ) delete Bitmap;
    Bitmap = new wxBitmap(Size.GetWidth(),Size.GetHeight());

    // Recreating drawing panel
    if ( Panel ) delete Panel;
    Panel = new DrawingPanel(this);
    Panel->Raise();

    // Resizing panel to cover whole window
    int X, Y;
    CalcScrolledPosition(0,0,&X,&Y);
    Panel->SetSize(X,Y,Size.GetWidth(),Size.GetHeight());

    // Background will be fetched inside panel's internal routines when showing this window
}

void wxsDrawingWindow::PanelPaint(wxPaintEvent& event)
{
#ifdef FETCHING_SYSTEM2
    WaitingForPaint = false;
    wxPaintDC PaintDC(Panel);
    if ( IsBlockFetch )
    {
//        DBGLOG(_T("Paint (Blocked)"));
        wxBitmap BmpCopy = Bitmap->GetSubBitmap(wxRect(0,0,Bitmap->GetWidth(),Bitmap->GetHeight()));
        wxBufferedDC DC(&PaintDC,BmpCopy);
        PaintExtra(&DC);
    }
    else
    {
//        if ( DuringFetch ) DBGLOG(_T("Paint (During fetch)"));
//        else               DBGLOG(_T("Paint (Raising fetch)"));
        StartFetchingSequence();
    }
#else
    if ( PaintAfterFetch || WaitTillHideChildren ||  IsBlockFetch )
    {
        wxPaintDC PaintDC(Panel);
        wxBitmap BmpCopy = Bitmap->GetSubBitmap(wxRect(0,0,Bitmap->GetWidth(),Bitmap->GetHeight()));
        wxBufferedDC DC(&PaintDC,BmpCopy);
        PaintExtra(&DC);
        if ( !IsBlockFetch && !WaitTillHideChildren )
        {
            PaintAfterFetch = false;
        }
    }
    else
    {
        StartFetchingSequence();
    }
#endif
}

void wxsDrawingWindow::PanelMouse(wxMouseEvent& event)
{
    event.SetEventObject(this);
    event.SetId(GetId());
    ProcessEvent(event);
}

void wxsDrawingWindow::PanelKeyboard(wxKeyEvent& event)
{
    event.SetEventObject(this);
    event.SetId(GetId());
    ProcessEvent(event);
}

void wxsDrawingWindow::StartFetchingSequence()
{
#ifdef FETCHING_SYSTEM2
    if ( DuringFetch )
    {
        return;
    }
    DuringFetch = true;
    // Fetching sequence will end after quitting
    // this event handler. This will be done
    // by adding some pending event
    wxCommandEvent event(wxEVT_FETCH_SEQUENCE,GetId());
    event.SetEventObject(this);
    GetEventHandler()->AddPendingEvent(event);
#else
    // This function will be blocking
    // If it has been executed and not yet finished,
    // another calls (possibly called from Manager::Yield())
    // will not be executed
    static bool Block = false;
    if ( Block ) return;
    Block = true;

    // Hiding panel to show content under it
    ShowChildren();
    Panel->Hide();

    // Processing all pending events, it MUST be done
    // to repaint the content of window
    Manager::Yield();
    FetchScreen();
    Manager::Yield();
    WaitTillHideChildren = true;
    HideChildren();
    Manager::Yield();
    WaitTillHideChildren = false;
    PaintAfterFetch = true;
    Manager::Yield();
    Panel->Raise();
    Manager::Yield();
    Panel->Show();
    Manager::Yield();

    Block = false;
#endif
}

#ifdef FETCHING_SYSTEM2
void wxsDrawingWindow::OnFetchSequence(wxCommandEvent& event)
{
//    static int Cnt = 1;
//    int CntLocal = ++Cnt;
//    DBGLOG(_T("=========================="));
//    DBGLOG(_T(""));
//    DBGLOG(_T("Fetch sequence started (%d)"),CntLocal);
//    DBGLOG(_T(""));
//    DBGLOG(_T("=========================="));

    // Hiding panel to show content under it
    Panel->Hide();
    ShowChildren();
    Update();
//    DBGLOG(_T("Children shown"));

    // Processing all pending events, it MUST be done
    // to repaint the content of window
    WaitingForPaint = true;
    Manager::Yield();
//    DBGLOG(_T("Fetching screen"));
    FetchScreen();
//    DBGLOG(_T("Hiding children"));
    HideChildren();
//    DBGLOG(_T("Raising panel"));
    Panel->Raise();
    Manager::Yield();
//    DBGLOG(_T("Showing panel"));
    Panel->Show();
    Manager::Yield();
//    DBGLOG(_T("Updating panel"));
    Panel->Update();
    Manager::Yield();

//    DBGLOG(_T("Full repaint"));
    FullRepaint();
    Manager::Yield();
//    Bitmap->SaveFile(wxString::Format(_T("c:/tmp%d.bmp"),CntLocal),wxBITMAP_TYPE_BMP);
    DuringFetch = false;
//    DBGLOG(_T("Fetch sequence finished (%d)"),CntLocal);
}
#endif

void wxsDrawingWindow::FetchScreen()
{
    // Fetching preview directly from screen
	wxScreenDC DC;
	wxMemoryDC DestDC;
    int X = 0, Y = 0;
    int DX = 0, DY = 0;
    ClientToScreen(&X,&Y);
    CalcUnscrolledPosition(0,0,&DX,&DY);
    DestDC.SelectObject(*Bitmap);
    DestDC.Blit(DX,DY,GetSize().GetWidth(),GetSize().GetHeight(),&DC,X,Y);
}

void wxsDrawingWindow::FullRepaint()
{
    wxClientDC ClientDC(Panel);
    wxBitmap BmpCopy = Bitmap->GetSubBitmap(wxRect(0,0,Bitmap->GetWidth(),Bitmap->GetHeight()));
    wxBufferedDC DC(&ClientDC,BmpCopy);
    PaintExtra(&DC);
}

void wxsDrawingWindow::ShowChildren()
{
    wxWindowList& Children = GetChildren();
    for ( size_t i=0; i<Children.GetCount(); i++ )
    {
        if ( Children[i] != Panel )
        {
            Children[i]->Show();
        }
    }
}

void wxsDrawingWindow::HideChildren()
{
    wxWindowList& Children = GetChildren();
    for ( size_t i=0; i<Children.GetCount(); i++ )
    {
        if ( Children[i] != Panel )
        {
            Children[i]->Hide();
        }
    }
}
