#ifdef __WXMSW__

#include "compilerCYGWIN.h"
#include <wx/filefn.h>
#include <wx/log.h>
#include <wx/msw/registry.h>

CompilerCYGWIN::CompilerCYGWIN()
    : CompilerMINGW(_("Cygwin GCC"), _T("cygwin"))
{
    Reset();
}

CompilerCYGWIN::~CompilerCYGWIN()
{
}

Compiler * CompilerCYGWIN::CreateCopy()
{
    Compiler* c = new CompilerCYGWIN(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

void CompilerCYGWIN::Reset()
{
    CompilerMINGW::Reset();

	m_Programs.C = _T("gcc.exe");
	m_Programs.CPP = _T("g++.exe");
	m_Programs.LD = _T("g++.exe");
	m_Programs.DBG = _T("gdb.exe");
	m_Programs.LIB = _T("ar.exe");
	m_Programs.WINDRES = _T("windres.exe");
	m_Programs.MAKE = _T("make.exe");

    m_Switches.forceFwdSlashes = true;

	m_Options.AddOption(_("Do not use cygwin specific functionality"), _T("-mno-cygwin"), _("General"));
}

AutoDetectResult CompilerCYGWIN::AutoDetectInstallationDir()
{
    m_MasterPath = _T("C:\\Cygwin"); // just a guess

    // look in registry for Cygwin

    wxLogNull ln;
    wxRegKey key; // defaults to HKCR
    key.SetName(_T("HKEY_LOCAL_MACHINE\\Software\\Cygnus Solutions\\Cygwin\\mounts v2\\/"));
    if (key.Open())
    {
        // found; read it
        key.QueryValue(_T("native"), m_MasterPath);
    }
    AutoDetectResult ret = wxFileExists(m_MasterPath + wxFILE_SEP_PATH +
                                        _T("bin") + wxFILE_SEP_PATH +
                                        m_Programs.C)
                            ? adrDetected
                            : adrGuessed;
    return ret;
}

#endif // __WXMSW__
