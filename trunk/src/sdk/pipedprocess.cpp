/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include <wx/app.h>         // wxWakeUpIdle
    #include "pipedprocess.h" // class' header file
    #include "sdk_events.h"
    #include "globals.h"
#endif

// The following class is created to override wxTextStream::ReadLine()
class cbTextInputStream : public wxTextInputStream
{
    protected:
        bool m_allowMBconversion;
    public:
#if wxUSE_UNICODE
        cbTextInputStream(wxInputStream& s, const wxString &sep=wxT(" \t"), wxMBConv& conv = wxConvLocal )
            : wxTextInputStream(s, sep, conv),
            m_allowMBconversion(true)
        {
            memset((void*)m_lastBytes, 0, 10);
        }
#else
        cbTextInputStream(wxInputStream& s, const wxString &sep=wxT(" \t") )
            : wxTextInputStream(s, sep)
        {
            memset((void*)m_lastBytes, 0, 10);
        }
#endif
        ~cbTextInputStream(){}


        // The following function was copied verbatim from wxTextStream::NextChar()
        // The only change, is the removal of the MB2WC function
        // With PipedProcess we work with compilers/debuggers which (usually) don't
        // send us unicode (at least GDB).
        wxChar NextChar()
        {
        #if wxUSE_UNICODE
            wxChar wbuf[2];
            memset((void*)m_lastBytes, 0, 10);
            for (size_t inlen = 0; inlen < 9; inlen++)
            {
                // actually read the next character
                m_lastBytes[inlen] = m_input.GetC();

                if (m_input.LastRead() <= 0)
                    return wxEOT;
                if (m_allowMBconversion)
                {
                    int retlen = (int) m_conv->MB2WC(wbuf, m_lastBytes, 2); // returns -1 for failure
                    if (retlen >= 0) // res == 0 could happen for '\0' char
                        return wbuf[0];
                }
                else
                    return m_lastBytes[inlen]; // C::B fix (?)
            }
            // there should be no encoding which requires more than nine bytes for one character...
            return wxEOT;
        #else
            m_lastBytes[0] = m_input.GetC();

            if (m_input.LastRead() <= 0)
                return wxEOT;

            return m_lastBytes[0];
        #endif
        }

        // The following function was copied verbatim from wxTextStream::ReadLine()
        // The only change, is the addition of m_input.CanRead() in the while()
        wxString ReadLine()
        {
            wxString line;

            while ( m_input.CanRead() && !m_input.Eof() )
            {
                wxChar c = NextChar();
                if (m_input.LastRead() <= 0)
                    break;

                if ( !m_input )
                    break;

                if (EatEOL(c))
                    break;

                line += c;
            }
            return line;
        }
};

int idTimerPollProcess = wxNewId();

BEGIN_EVENT_TABLE(PipedProcess, wxProcess)
    EVT_TIMER(idTimerPollProcess, PipedProcess::OnTimer)
    EVT_IDLE(PipedProcess::OnIdle)
END_EVENT_TABLE()

// class constructor
PipedProcess::PipedProcess(PipedProcess** pvThis, wxEvtHandler* parent, int id, bool pipe,
                           const wxString& dir, int index)
    : wxProcess(parent, id),
    m_Parent(parent),
    m_Id(id),
    m_Pid(0),
    m_Index(index),
    m_Stopped(false),
    m_pvThis(pvThis)
{
    const wxString &unixDir = UnixFilename(dir);
    if (!unixDir.empty())
        wxSetWorkingDirectory(unixDir);
    if (pipe)
        Redirect();

    m_timerPollProcess.SetOwner(this, idTimerPollProcess);
}

// class destructor
PipedProcess::~PipedProcess()
{
    if (m_timerPollProcess.IsRunning())
        m_timerPollProcess.Stop();
}

int PipedProcess::Launch(const wxString& cmd,int flags ,cb_unused unsigned int pollingInterval)
{
    m_Stopped = false;
    m_Pid = wxExecute(cmd, wxEXEC_ASYNC | wxEXEC_MAKE_GROUP_LEADER, this);
    if (m_Pid)
    {
        m_timerPollProcess.Start(pollingInterval);
    }
    return m_Pid;
}

void PipedProcess::SendString(const wxString& text)
{
    //Manager::Get()->GetLogManager()->Log(m_PageIndex, cmd);
    wxOutputStream* pOut = GetOutputStream();
    if (pOut)
    {
        wxTextOutputStream sin(*pOut);
        wxString msg = text + _T('\n');
        sin.WriteString(msg);
    }
}

void PipedProcess::ForfeitStreams()
{
    m_Stopped = true;
    char buf[4096];
    if ( IsErrorAvailable() )
    {
        wxInputStream *in = GetErrorStream();
        while (in->Read(&buf, sizeof(buf)).LastRead())
            ;
    }
    if ( IsInputAvailable() )
    {
        wxInputStream *in = GetInputStream();
        while (in->Read(&buf, sizeof(buf)).LastRead())
            ;
    }
}

bool PipedProcess::HasInput()
{
    if (IsErrorAvailable())
    {
        cbTextInputStream serr(*GetErrorStream());

        wxString msg;
        msg << serr.ReadLine();

        if (m_Stopped)
            return true;

        CodeBlocksEvent event(cbEVT_PIPEDPROCESS_STDERR, m_Id);
        event.SetString(msg);
        event.SetX(m_Index);
        wxPostEvent(m_Parent, event);

        return true;
    }

    if (IsInputAvailable())
    {
        cbTextInputStream sout(*GetInputStream());

        wxString msg;
        msg << sout.ReadLine();

        if (m_Stopped)
            return true;

        CodeBlocksEvent event(cbEVT_PIPEDPROCESS_STDOUT, m_Id);
        event.SetString(msg);
        event.SetX(m_Index);
        wxPostEvent(m_Parent, event);

        return true;
    }

    return false;
}

void PipedProcess::OnTerminate(int /*pid*/, int status)
{
    // show the rest of the output
    while ( HasInput() )
        ;

    CodeBlocksEvent event(cbEVT_PIPEDPROCESS_TERMINATED, m_Id);
    event.SetInt(status);
    event.SetX(m_Index);
    wxPostEvent(m_Parent, event);

    if (m_pvThis)
        *m_pvThis = nullptr;
    delete this;
}

void PipedProcess::OnTimer(cb_unused wxTimerEvent& event)
{
    wxWakeUpIdle();
}

void PipedProcess::OnIdle(wxIdleEvent& event)
{
    while ( HasInput() )
        ;
    event.Skip();
}
