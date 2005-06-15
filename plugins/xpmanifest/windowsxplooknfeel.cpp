/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* Contact e-mail: Yiannis An. Mandravellos <mandrav@codeblocks.org>
* Program URL   : http://www.codeblocks.org
*
* $Id$
* $Date$
*/

#include <wx/intl.h>
#include <wx/file.h>
#include <wx/msgdlg.h>
#include <manager.h>
#include <cbproject.h>
#include <projectmanager.h>
#include <messagemanager.h>
#include <licenses.h>
#include "windowsxplooknfeel.h"

cbPlugin* GetPlugin()
{
	return new WindowsXPLookNFeel;
}

WindowsXPLookNFeel::WindowsXPLookNFeel()
{
	//ctor
	m_PluginInfo.name = "WindowsXPLookNFeel";
	m_PluginInfo.title = "Windows XP Look'n'Feel";
	m_PluginInfo.version = "1.0";
	m_PluginInfo.description = "This plugin creates a manifest file that makes "
                               "use of common controls 6.0 under Windows XP. "
                               "You must copy this manifest file in the same "
                               "path as your app's main executable in order for "
                               "your application to use common controls version 6.0...";
    m_PluginInfo.author = "Yiannis An. Mandravellos";
    m_PluginInfo.authorEmail = "info@codeblocks.org";
    m_PluginInfo.authorWebsite = "www.codeblocks.org";
	m_PluginInfo.thanksTo = "";
	m_PluginInfo.license = LICENSE_GPL;
	m_PluginInfo.hasConfigure = false;
}

WindowsXPLookNFeel::~WindowsXPLookNFeel()
{
	//dtor
}

void WindowsXPLookNFeel::OnAttach()
{
	// do whatever initialization you need for your plugin
	// NOTE: after this function, the inherited member variable
	// m_IsAttached will be TRUE...
	// You should check for it in other functions, because if it
	// is FALSE, it means that the application did *not* "load"
	// (see: does not need) this plugin...
}

void WindowsXPLookNFeel::OnRelease(bool appShutDown)
{
	// do de-initialization for your plugin
	// NOTE: after this function, the inherited member variable
	// m_IsAttached will be FALSE...
}

int WindowsXPLookNFeel::Execute()
{
	if (!m_IsAttached)
		return -1;

#ifndef __WXMSW__
	#error This plugin is for use under Win32 only!
#endif // __WXMSW__

	cbProject* project = Manager::Get()->GetProjectManager()->GetActiveProject();
	if (!project)
	{
		wxString msg = _("No active project!");
		wxMessageBox(msg, _("Error"), wxICON_ERROR | wxOK);
		Manager::Get()->GetMessageManager()->DebugLog(msg);
		return -1;
	}
	
	wxArrayString targetNames;
	ProjectBuildTarget* target = 0L;
	for (int i = 0; i < project->GetBuildTargetsCount(); ++i)
	{
		ProjectBuildTarget* tgt = project->GetBuildTarget(i);
		if (tgt)
		{
			if (tgt->GetTargetType() != ttExecutable)
			{
				Manager::Get()->GetMessageManager()->DebugLog(_("WindowsXPLookNFeel: Ignoring target '%s'"), tgt->GetTitle().c_str());
				continue;
			}
			targetNames.Add(tgt->GetTitle());
			target = tgt;
		}
	}
	
	if (!target)
	{
		// not even one executable target...
		Manager::Get()->GetMessageManager()->DebugLog(_("WindowsXPLookNFeel: No executable targets in project"));
		return -1;
	}
	else if (targetNames.GetCount() > 1)
	{
		// more than one executable target... ask...
		target = 0L;
		int targetIndex = project->SelectTarget(-1, true);
		if (targetIndex > -1)
			target = project->GetBuildTarget(targetIndex);
	}
	
	
	if (target)
	{
		if (wxMessageBox(_("Do you want to create the manifest file?"),
						_("Confirmation"),
						wxYES_NO | wxICON_QUESTION) == wxNO)
			return -2;
		wxString filename = target->GetOutputFilename();
		filename << ".Manifest";
		wxFileName fname(filename);
		fname.Normalize(wxPATH_NORM_ALL & ~wxPATH_NORM_CASE, project->GetBasePath());
		filename = fname.GetFullPath();
		Manager::Get()->GetMessageManager()->DebugLog(_("WindowsXPLookNFeel: Creating Manifest '%s'"), filename.c_str());
		
		wxString buffer;
		buffer << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" << '\n';
		buffer << "<assembly" << '\n';
		buffer << "  xmlns=\"urn:schemas-microsoft-com:asm.v1\"" << '\n';
		buffer << "  manifestVersion=\"1.0\">" << '\n';
		buffer << "<assemblyIdentity" << '\n';
		buffer << "    name=\"";
		buffer << project->GetTitle() << "." << target->GetTitle() << ".App";
		buffer << "\"" << '\n';
		buffer << "    processorArchitecture=\"x86\"" << '\n';
		buffer << "    version=\"1.0.0.0\"" << '\n';
		buffer << "    type=\"win32\"/>" << '\n';
		buffer << "<description>Executable</description>" << '\n';
		buffer << "<dependency>" << '\n';
		buffer << "    <dependentAssembly>" << '\n';
		buffer << "        <assemblyIdentity" << '\n';
		buffer << "            type=\"win32\"" << '\n';
		buffer << "            name=\"Microsoft.Windows.Common-Controls\"" << '\n';
		buffer << "            version=\"6.0.0.0\"" << '\n';
		buffer << "            processorArchitecture=\"x86\"" << '\n';
		buffer << "            publicKeyToken=\"6595b64144ccf1df\"" << '\n';
		buffer << "            language=\"*\"" << '\n';
		buffer << "        />" << '\n';
		buffer << "    </dependentAssembly>" << '\n';
		buffer << "</dependency>" << '\n';
		buffer << "</assembly>" << '\n';
		
		wxFile file(filename, wxFile::write);
		file.Write(buffer);

		wxMessageBox(_("Manifest file created"));
	}
	
	return 0;
}

