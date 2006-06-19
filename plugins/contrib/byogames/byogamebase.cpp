#include <sdk.h>
#include "byogamebase.h"

#include <annoyingdialog.h>
#include <wx/artprov.h>
#include <messagemanager.h>


byoGameBase::GamesListT byoGameBase::AllGames;

namespace
{
    const wxColour colours[] =
    {
        wxColour(0xFF,0,0),
        wxColour(0,0xFF,0),
        wxColour(0,0,0xFF),
        wxColour(0xFF,0xFF,0),
        wxColour(0xFF,0,0xFF),
        wxColour(0,0xFF,0xFF)
    };

    const int coloursCount = sizeof(colours)/sizeof(colours[0]);

    long PlayingTicks = 0;
    long WorkingTicks = 0;
    bool PlayBlocked = false;
    int  PlayingCount = 0;

    int  MaxPlayTime = 10*60;
    int  MinWorkTime = 60*60;
    int  MaxWorkTime = 3*60*60;
}

BEGIN_EVENT_TABLE(byoGameBase,wxWindow)
    EVT_SIZE(byoGameBase::OnSize)
    EVT_KILL_FOCUS(byoGameBase::OnKillFocus)
END_EVENT_TABLE()

byoGameBase::byoGameBase(wxWindow* parent,const wxString& GameName):
    m_CellSize(10),
    m_FirstCellXPos(0),
    m_FirstCellYPos(0),
    m_MinCellsHoriz(10),
    m_MinCellsVert(10),
    m_Paused(true),
    m_GameName(GameName)
{
	Create(parent,-1,wxDefaultPosition,wxDefaultSize,wxWANTS_CHARS);
    AllGames.Add(this);
    SetPause(false);
}

byoGameBase::~byoGameBase()
{
    AllGames.Remove(this);
}

void byoGameBase::ReloadFromConfig()
{
    // TODO: Load colours etc.
}

void byoGameBase::BackToWorkTimer()
{
    if ( PlayingCount > 0 )
    {
        PlayingTicks++;
        if ( PlayingTicks >= MaxPlayTime )
        {
            // Played to much

            PlayBlocked = true;
            for ( size_t i=0; i<AllGames.Count(); i++ )
            {
                AllGames[i]->SetPause(true);
            }
            WorkingTicks = 0;

            AnnoyingDialog dlg(_("Work reminder (stop playing games!)"),
                               _("Don't you think you had enough already?\nGet back to work, NOW!"),
                               wxART_WARNING,
                               AnnoyingDialog::OK,
                               wxID_OK);
            dlg.ShowModal();
        }
    }
    else if ( PlayBlocked )
    {
        WorkingTicks++;
        if ( WorkingTicks >= MinWorkTime )
        {
            PlayBlocked = false;
            PlayingTicks = 0;
        }
    }
    else
    {
        WorkingTicks++;
        if ( WorkingTicks >= MaxWorkTime )
        {

            AnnoyingDialog dlg(_("Repose reminder"),
                               _("You've been working for a long time.\n"
                                 "Please stand up, take small walk,\n"
                                 "make tea or cofee, smile to your neighbours :)\n"
                                 "\n"
                                 "I'm watching you, do not cheat\n"),
                               wxART_WARNING,
                               AnnoyingDialog::OK,
                               wxID_OK);
            dlg.ShowModal();
            WorkingTicks = 0;
        }
    }

    for ( size_t i=0; i<AllGames.Count(); i++ )
    {
        AllGames[i]->Refresh();
    }
}

void byoGameBase::RecalculateSizeHints(int minStepsHoriz,int minStepsVert)
{
    wxSize size = GetClientSize();

    int cellSizeH = size.GetWidth() / minStepsHoriz;
    int cellSizeV = size.GetHeight() / minStepsVert;

    m_CellSize = (cellSizeH > cellSizeV) ? cellSizeV : cellSizeH;
    if ( m_CellSize < 3 ) m_CellSize = 3;

    m_FirstCellXPos = (size.GetWidth()  - minStepsHoriz*m_CellSize) / 2;
    m_FirstCellYPos = (size.GetHeight() - minStepsVert *m_CellSize) / 2;

    m_MinCellsHoriz = minStepsHoriz;
    m_MinCellsVert  = minStepsVert;

    DBGLOG(_T("msh: %d, msv: %d, ch: %d, cv: %d, cs: %d, x: %d, y: %d"),
        minStepsHoriz,
        minStepsVert,
        cellSizeH,
        cellSizeV,
        m_CellSize,
        m_FirstCellXPos,
        m_FirstCellYPos);

}

void byoGameBase::GetCellAbsolutePos(int cellX,int cellY,int& posX,int& posY)
{
    posX = m_FirstCellXPos + cellX * m_CellSize;
    posY = m_FirstCellYPos + cellY * m_CellSize;
}

void byoGameBase::DrawBrickAbsolute(wxDC* DC,int posX,int posY,int width,int height,const wxColour& colour)
{
    wxColour darker(colour.Red()/2,colour.Green()/2,colour.Blue()/2);
    wxColour brighter(darker.Red()+0x80,darker.Green()+0x80,darker.Blue()+0x80);

    DC->SetPen(brighter);
    DC->SetBrush(colour);
    DC->DrawRectangle(posX,posY,width,height);

    int borderSize = (width + height) / 16;
    if ( borderSize < 1 ) borderSize = 1;

    for ( int i=0; i<borderSize; i++ )
    {
        DC->SetPen(brighter);
        DC->DrawLine(posX+i,posY+i,posX+width-i,posY+i);
        DC->DrawLine(posX+i,posY+i,posX+i,posY+height-i);
        DC->SetPen(darker);
        DC->DrawLine(posX+width-1-i,posY+height-1-i,posX+i-1,posY+height-1-i);
        DC->DrawLine(posX+width-1-i,posY+height-1-i,posX+width-1-i,posY+i);
    }
}

void byoGameBase::DrawBrick(wxDC* DC,int cellX,int cellY,const wxColour& base)
{
    int posX=0, posY=0;
    GetCellAbsolutePos(cellX,cellY,posX,posY);
    DrawBrickAbsolute(DC,posX,posY,m_CellSize,m_CellSize,base);
}

const wxColour& byoGameBase::GetColour(int index)
{
    return colours[index%coloursCount];
}

bool byoGameBase::SetPause(bool pause)
{
    if ( m_Paused == pause ) return pause;
    if ( !pause && !PlayBlocked )
    {
        PlayingCount++;
        m_Paused = false;
    }
    if ( pause )
    {
        PlayingCount--;
        m_Paused = true;
    }
    return m_Paused;
}

wxString byoGameBase::GetBackToWorkString()
{
    if ( !PlayBlocked ) return wxEmptyString;
    int left = MinWorkTime - WorkingTicks;
    return wxString::Format(_("Please wait... %d:%d left"),left/60,left%60);
}

void byoGameBase::OnSize(wxSizeEvent& event)
{
    RecalculateSizeHints(m_MinCellsHoriz,m_MinCellsVert);
}

void byoGameBase::OnKillFocus(wxFocusEvent& event)
{
    SetPause(true);
}
