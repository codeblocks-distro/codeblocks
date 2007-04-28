/////////////////////////////////////////////////////////////////////////////
// Name:        wxBitmapComboBox
// Purpose:     A wxComboBox type button for bitmaps and strings
// Author:      John Labenski
// Modified by:
// Created:     11/05/2002
// RCS-ID:
// Copyright:   (c) John Labenki
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "bmpcombo.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/control.h"
    #include "wx/menu.h"
    #include "wx/settings.h"
    #include "wx/bitmap.h"
    #include "wx/dc.h"
    #include "wx/dcclient.h"
    #include "wx/scrolwin.h"
#endif // WX_PRECOMP

#include "wx/things/bmpcombo.h"

#define BORDER 4

// ============================================================================
// wxBitmapComboPopupChild
// ============================================================================
BEGIN_EVENT_TABLE(wxBitmapComboPopupChild, wxScrolledWindow)
    EVT_PAINT( wxBitmapComboPopupChild::OnPaint )
    EVT_MOUSE_EVENTS( wxBitmapComboPopupChild::OnMouse )
    EVT_KEY_DOWN( wxBitmapComboPopupChild::OnKeyDown )
END_EVENT_TABLE()

wxBitmapComboPopupChild::wxBitmapComboPopupChild(wxWindow *parent, wxBitmapComboBox *owner)
                        : wxScrolledWindow( parent, wxID_ANY, wxDefaultPosition,
                             wxDefaultSize, wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL)
{
    m_bmpCombo = owner;
    m_last_selection = -1;
    SetBackgroundColour(m_bmpCombo->GetBackgroundColour());
}

void wxBitmapComboPopupChild::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
    wxPaintDC dc(this);
    PrepareDC(dc);
    //dc.SetBackground(*wxTheBrushList->FindOrCreateBrush(GetBackgroundColour(), wxSOLID));
    //dc.Clear();

    dc.SetFont(m_bmpCombo->GetFont());

    int y = 0, dy = m_bmpCombo->GetItemSize().y;
    wxPoint origin = dc.GetDeviceOrigin();
    wxSize clientSize = GetClientSize();

    for (int n=0; n<m_bmpCombo->GetCount(); n++)
    {
        if (y + dy > -origin.y)
        {
            dc.SetDeviceOrigin(origin.x, origin.y + y + 1);
            m_bmpCombo->DrawItem(dc, n);
        }

        y += dy;
        if (y > -origin.y + clientSize.y)
            break;
    }

    dc.SetDeviceOrigin(0, 0);
    PrepareDC(dc); // reset back

    if (m_bmpCombo->GetSelection() >= 0)
    {
        if (m_last_selection < 0)
            m_last_selection = m_bmpCombo->GetSelection();

        DrawSelection(m_last_selection, dc);
    }
}

void wxBitmapComboPopupChild::OnMouse( wxMouseEvent &event )
{
    wxPoint mouse = event.GetPosition();
    CalcUnscrolledPosition(mouse.x, mouse.y, &mouse.x, &mouse.y);

    //wxPrintf(wxT("bmpcombo mouse %d %d\n"), mouse.x, mouse.y); fflush(stdout);

    // Get selection from mouse pos, force valid
    int sel = m_bmpCombo->GetItemSize().y != 0 ? mouse.y/m_bmpCombo->GetItemSize().y : -1;
    if (sel < 0)
        sel = 0;
    else if (sel >= m_bmpCombo->GetCount())
        sel = m_bmpCombo->GetCount()-1;

    if (event.LeftDown())
    {
        // quickly show user what they selected before hiding it
        if (sel != m_last_selection)
        {
            wxClientDC dc(this);
            PrepareDC(dc);
            if (m_last_selection >= 0)
                DrawSelection(m_last_selection, dc);
            if (sel >= 0)
                DrawSelection(sel, dc);

            m_last_selection = sel;
        }

        m_bmpCombo->SetSelection(sel, true);
        m_bmpCombo->HidePopup();
        return;
    }
}

void wxBitmapComboPopupChild::OnKeyDown( wxKeyEvent &event )
{
    int sel = m_last_selection;

    switch (event.GetKeyCode())
    {
        case WXK_ESCAPE :
        {
            m_bmpCombo->HidePopup();
            return;
        }
        case WXK_RETURN :
        {
            m_bmpCombo->SetSelection(sel, true);
            m_bmpCombo->HidePopup();
            return;
        }
        case WXK_UP     : sel--; break;
        case WXK_DOWN   : sel++; break;
        default : event.Skip(true); return;
    }

    if (sel < 0)
        sel = 0;
    if (sel >= m_bmpCombo->GetCount())
        sel = m_bmpCombo->GetCount()-1;

    if (sel != m_last_selection)
    {
        wxClientDC dc(this);
        PrepareDC(dc);
        if (m_last_selection>=0)
            DrawSelection(m_last_selection, dc);

        if (sel>=0)
            DrawSelection(sel, dc);

        m_last_selection = sel;
    }
}

void wxBitmapComboPopupChild::DrawSelection( int n, wxDC& dc )
{
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetPen(*wxBLACK_PEN);
    dc.SetLogicalFunction(wxINVERT);
    int height = m_bmpCombo->GetItemSize().y;
    dc.DrawRectangle(0, wxMax(0,height*n-1), GetClientSize().x, height+2);
    dc.SetLogicalFunction(wxCOPY);
}

// ==========================================================================
// wxBitmapComboLabel - the main "window" to the left of the dropdown button
// ==========================================================================
IMPLEMENT_DYNAMIC_CLASS( wxBitmapComboLabel, wxWindow )

BEGIN_EVENT_TABLE(wxBitmapComboLabel, wxWindow)
    EVT_PAINT( wxBitmapComboLabel::OnPaint )
    EVT_CHAR( wxBitmapComboLabel::OnChar )
END_EVENT_TABLE()

void wxBitmapComboLabel::OnChar( wxKeyEvent &event )
{
    switch (event.GetKeyCode())
    {
        case WXK_UP   : m_bmpCombo->SetNextSelection(false, true); break;
        case WXK_DOWN : m_bmpCombo->SetNextSelection(true, true); break;
        default : break;
    }
}

void wxBitmapComboLabel::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
    wxPaintDC dc(this);
    dc.SetFont(m_bmpCombo->GetFont());
    //dc.SetBackground(*wxTheBrushList->FindOrCreateBrush(GetBackgroundColour(), wxSOLID));
    //dc.Clear();
    dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(GetBackgroundColour(), wxSOLID));
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawRectangle(wxRect(wxPoint(0,0), GetClientSize()));

    const int sel = m_bmpCombo->GetSelection();
    if ((sel >= 0) && (sel < m_bmpCombo->GetCount()))
        m_bmpCombo->DrawItem(dc, sel);
}

// ============================================================================
// wxBitmapComboBox
// ============================================================================
IMPLEMENT_DYNAMIC_CLASS( wxBitmapComboBox, DropDownBase )

BEGIN_EVENT_TABLE(wxBitmapComboBox,DropDownBase)
    EVT_SIZE( wxBitmapComboBox::OnSize )
END_EVENT_TABLE()

wxBitmapComboBox::~wxBitmapComboBox()
{
    while (m_bitmaps.GetCount() > 0u)
    {
        wxBitmap *bmp = (wxBitmap*)m_bitmaps.Item(0);
        m_bitmaps.RemoveAt(0);
        delete bmp;
    }
}

void wxBitmapComboBox::Init()
{
    m_labelWin = NULL;
    m_frozen = true;
    m_selection = 0;
    m_win_border = 0;
    m_label_style = wxBMPCOMBO_LEFT;
}

bool wxBitmapComboBox::Create( wxWindow* parent, wxWindowID id,
                               const wxPoint& pos, const wxSize& size,
                               long style, const wxValidator& val,
                               const wxString& name)
{
    if (!DropDownBase::Create(parent,id,pos,size,wxNO_BORDER|wxCLIP_CHILDREN,val,name))
        return false;

    m_labelWin = new wxBitmapComboLabel(this);
    m_win_border = m_labelWin->GetSize().x - m_labelWin->GetClientSize().x;

    SetBackgroundColour(*wxWHITE);

    m_frozen = false;
    CalcLayout();

    wxSize bestSize = DoGetBestSize();
    SetSize( wxSize(size.x < 0 ? bestSize.x : size.x,
                    size.y < 0 ? bestSize.y : size.y) );

    return SetButtonStyle(style);
}

#define BMPCOMBO_LABEL_MASK (wxBMPCOMBO_LEFT|wxBMPCOMBO_RIGHT)

bool wxBitmapComboBox::SetButtonStyle(long style)
{
    style &= BMPCOMBO_LABEL_MASK; // strip off extras

    int n_styles = 0;
    if (style & wxBMPCOMBO_LEFT) n_styles++;
    if (style & wxBMPCOMBO_RIGHT) n_styles++;
    wxCHECK_MSG(n_styles < 2, false, wxT("Only one wxBitmapComboBox label position allowed"));
    if (n_styles < 1) style |= (m_label_style & BMPCOMBO_LABEL_MASK);

    m_label_style = style;

    m_labelWin->Refresh(true);

    return true;
}

void wxBitmapComboBox::OnSize( wxSizeEvent& event )
{
    event.Skip();

    if (!m_labelWin || !m_dropdownButton) return;

	wxSize size = GetClientSize();
    //wxPrintf(wxT("ComboOnSize %d %d\n"), size.x, size.y);
	int width = size.x - ((wxWindow*)m_dropdownButton)->GetSize().x;
    m_labelWin->SetSize(0, 0, width, size.y);
}

void wxBitmapComboBox::DoSetSize(int x, int y, int width, int height, int sizeFlags)
{
/*
    wxSize curSize( GetSize() );

    if (width == -1)
        width = curSize.GetWidth();
    if (height == -1)
        height = curSize.GetHeight();
*/
    DropDownBase::DoSetSize(x, y, width, height, sizeFlags);
/*
	width = width - ((wxWindow*)m_dropdownButton)->GetSize().x;
    m_labelWin->SetSize(0, 0, width, height);
*/
}

wxSize wxBitmapComboBox::DoGetBestSize() const
{
    if (GetCount() == 0) return DropDownBase::DoGetBestSize();

    wxSize size(0,0);
    size.x = m_labelSize.x + m_bitmapSize.x + (m_labelSize.x != 0 ? BORDER*2 : 0);
    size.y = wxMax(m_labelSize.y, m_bitmapSize.y) + m_win_border;

    size.x += m_win_border + DROPDOWN_DROP_WIDTH;
    if (size.y < DROPDOWN_DROP_HEIGHT) size.y = DROPDOWN_DROP_HEIGHT;

    return size;
}

int wxBitmapComboBox::DoGetBestDropHeight(int max_height)
{
    int count = GetCount();
    if (count < 1) return -1;

    // add one for drawing selection rect
    return wxMin(m_itemSize.y*count + m_win_border+1, max_height);
}

bool wxBitmapComboBox::DoShowPopup()
{
    if (m_popupWin)
    {
        wxBitmapComboPopupChild *popChild = new wxBitmapComboPopupChild(m_popupWin, this);
        m_popupWin->SetChild(popChild);

        if (popChild)
        {
            popChild->m_last_selection = GetSelection();
            int scr_pos = m_selection > 0 ? m_selection*m_itemSize.y-1 : 0;
            int count = GetCount();
            popChild->SetScrollbars(1, 1, m_itemSize.x, m_itemSize.y*count+1, 0, scr_pos);
        }
    }

    return DropDownBase::DoShowPopup();
}

void wxBitmapComboBox::HidePopup()
{
    DropDownBase::HidePopup();

    // FIXME - MSW destroys the sunken border of labelWin when in toolbar
    //         a refresh doesn't help
}

void wxBitmapComboBox::Thaw()
{
    m_frozen = false;
    CalcLayout();
    if (m_labelWin)
        m_labelWin->Refresh();
}

void wxBitmapComboBox::CalcLayout()
{
    if (m_frozen) return;

    int height = 0, width = 0;
    m_itemSize = m_labelSize = m_bitmapSize = wxSize(0,0);
    int count  = GetCount();
    wxBitmap bmp;

    for (int n=0; n<count; n++)
    {
        bmp = GetBitmap(n);
        if (bmp.Ok())
        {
            width  = bmp.GetWidth();
            height = bmp.GetHeight();

            if (width  > m_bitmapSize.x) m_bitmapSize.x = width;
            if (height > m_bitmapSize.y) m_bitmapSize.y = height;
        }
        if (!m_labels[n].IsEmpty())
        {
            GetTextExtent(m_labels[n], &width, &height);

            if (width  > m_labelSize.x) m_labelSize.x = width;
            if (height > m_labelSize.y) m_labelSize.y = height;
        }
    }

    m_itemSize.x = m_labelSize.x + m_bitmapSize.x + m_win_border;
    m_itemSize.y = wxMax(m_labelSize.y, m_bitmapSize.y) + m_win_border;
}

void wxBitmapComboBox::CalcLabelBitmapPos(int n, const wxSize &area, wxPoint &labelPos, wxPoint &bitmapPos) const
{
    labelPos = bitmapPos = wxPoint(0,0);

    int bw = 0, bh = 0;
    int lw = 0, lh = 0;

    if (GetBitmap(n).Ok())
    {
        bw = GetBitmap(n).GetWidth();
        bh = GetBitmap(n).GetHeight();
    }
    if (!m_labels[n].IsEmpty())
    {
        GetTextExtent(m_labels[n], &lw, &lh);
    }

    if (m_bitmapSize.x == 0)      // There aren't any bitmaps, left align label
    {
        labelPos = wxPoint(BORDER, (area.y-lh)/2);
    }
    else if (m_labelSize.x == 0)  // There aren't any labels, center bitmap
    {
        bitmapPos = wxPoint((area.x-bw)/2, (area.y-bh)/2);
    }
    else if ((m_label_style & wxBMPCOMBO_RIGHT) != 0)
    {
        labelPos = wxPoint(m_bitmapSize.x+BORDER, (area.y - lh)/2);
        bitmapPos = wxPoint((m_bitmapSize.x-bw)/2, (area.y - bh)/2);
    }
    else // if ((m_label_style & wxBMPCOMBO_LEFT) != 0)
    {
        labelPos = wxPoint(BORDER, (area.y - lh)/2);
        bitmapPos = wxPoint(BORDER*2 + m_labelSize.x + (area.x - BORDER*2 - m_labelSize.x - bw)/2, (area.y - bh)/2);
    }
}

void wxBitmapComboBox::DrawItem(wxDC &dc, int n) const
{
    wxSize itemSize(GetItemSize()); //((wxWindow*)GetLabelWindow())->GetClientSize().x, dy);

    wxPoint labelPos, bitmapPos;
    CalcLabelBitmapPos(n, itemSize, labelPos, bitmapPos);

    if (GetBitmap(n).Ok())
        dc.DrawBitmap(GetBitmap(n), bitmapPos.x, bitmapPos.y, true);
    if (!GetLabel(n).IsEmpty())
        dc.DrawText(GetLabel(n), labelPos.x, labelPos.y);
}

void wxBitmapComboBox::Append(const wxString &label, const wxBitmap &bitmap, int count)
{
    for (int n=0; n<count; n++)
    {
        m_labels.Add(label);
        m_bitmaps.Add(new wxBitmap(bitmap));
    }
    CalcLayout();
}

void wxBitmapComboBox::Insert(int n, const wxString &label, const wxBitmap &bitmap)
{
    wxCHECK_RET((n>=0) && (n<GetCount()), wxT("invalid index"));

    m_labels.Insert(label, n);
    m_bitmaps.Insert(new wxBitmap(bitmap), n);
    CalcLayout();
}

void wxBitmapComboBox::Clear()
{
    m_labels.Clear();
    while (m_bitmaps.GetCount() > 0u)
    {
        wxBitmap *bmp = (wxBitmap*)m_bitmaps.Item(0);
        m_bitmaps.RemoveAt(0);
        delete bmp;
    }
    CalcLayout();
}

void wxBitmapComboBox::Delete( int n, int count )
{
    wxCHECK_RET((n>=0) && (count>0) && (n+count<=GetCount()), wxT("invalid index"));

    for (int i=0; i<count; i++)
    {
        m_labels.RemoveAt(n);
        wxBitmap *bmp = (wxBitmap*)m_bitmaps.Item(n);
        m_bitmaps.RemoveAt(n);
        delete bmp;
    }
    CalcLayout();
}

wxString wxBitmapComboBox::GetLabel( int n ) const
{
    wxCHECK_MSG((n>=0) && (n < GetCount()), wxEmptyString, wxT("invalid index"));
    return m_labels[n];
}

wxBitmap wxBitmapComboBox::GetBitmap( int n ) const
{
    wxCHECK_MSG((n>=0) && (n < GetCount()), wxNullBitmap, wxT("invalid index"));
    return *(wxBitmap *)m_bitmaps.Item(n);
}

void wxBitmapComboBox::SetSelection( int n, bool send_event )
{
    wxCHECK_RET((n>=0) && (n < GetCount()), wxT("invalid index"));
    m_selection = n;
    m_labelWin->Refresh(true);

    if (send_event)
    {
        wxCommandEvent event( wxEVT_COMMAND_COMBOBOX_SELECTED, GetId() );
        event.SetInt( m_selection );
        event.SetEventObject( this );
        GetEventHandler()->ProcessEvent( event );
    }
}

void wxBitmapComboBox::SetNextSelection(bool foward, bool send_event)
{
    const int count = GetCount();
    if (count == 0) return;

    int sel = m_selection;

    if (foward)
    {
        if ((sel < 0) || (sel == count - 1))
            sel = 0;
        else
            sel++;
    }
    else
    {
        if (sel <= 0)
            sel = count - 1;
        else
            sel--;
    }

    SetSelection(sel, send_event);
}

void wxBitmapComboBox::SetLabel(int n, const wxString &label)
{
    wxCHECK_RET( (n>=0) && (n < GetCount()), wxT("invalid index"));
    m_labels[n] = label;
    CalcLayout();

    if (n == m_selection) m_labelWin->Refresh(false);
}

void wxBitmapComboBox::SetBitmap(int n, const wxBitmap &bitmap)
{
    wxCHECK_RET((n>=0) && (n < GetCount()), wxT("invalid index"));
    *((wxBitmap*)m_bitmaps.Item(n)) = bitmap;
    CalcLayout();

    if (n == m_selection) m_labelWin->Refresh(false);
}

bool wxBitmapComboBox::SetBackgroundColour(const wxColour &colour)
{
    // not a failure for wx 2.5.x since InheritAttributes calls this
    // from wxWindow::Create
    if (m_labelWin)
    {
        m_labelWin->SetBackgroundColour(colour);
        m_labelWin->Refresh();
    }
    return DropDownBase::SetBackgroundColour(colour);
}
