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

#include "classwizard.h"
#include <wx/intl.h>
#include <wx/filename.h>
#include <wx/xrc/xmlres.h>
#include <wx/fs_zip.h>
#include <wx/mdi.h>
#include <wx/msgdlg.h>
#include <manager.h>
#include <configmanager.h>
#include <projectmanager.h>
#include "classwizarddlg.h"

cbPlugin* GetPlugin()
{
    return new ClassWizard;
}


ClassWizard::ClassWizard()
{
    wxFileSystem::AddHandler(new wxZipFSHandler);
    wxXmlResource::Get()->InitAllHandlers();
    wxString resPath = ConfigManager::Get()->Read("data_path", wxEmptyString);
    wxXmlResource::Get()->Load(resPath + "/class_wizard.zip");

    m_PluginInfo.name = "ClassWizard";
    m_PluginInfo.title = "Class wizard";
    m_PluginInfo.version = "0.1";
    m_PluginInfo.description = "This plugin provides an easy way to create a "
                               "new C++ class file pair. It's by no means "
                               "complete yet but it's here nevertheless;)";
    m_PluginInfo.author = "Yiannis An. Mandravellos";
    m_PluginInfo.authorEmail = "info@codeblocks.org";
    m_PluginInfo.authorWebsite = "www.codeblocks.org";
    m_PluginInfo.thanksTo = "";
	m_PluginInfo.hasConfigure = false;
}

ClassWizard::~ClassWizard()
{
}

void ClassWizard::OnAttach()
{
}

void ClassWizard::OnRelease()
{
}

int ClassWizard::Execute()
{
	ClassWizardDlg dlg(Manager::Get()->GetAppWindow());
	if (dlg.ShowModal() == wxID_OK)
	{
		ProjectManager* prjMan = Manager::Get()->GetProjectManager();
		cbProject* prj = prjMan->GetActiveProject();
		if (!prj)
		{
			wxMessageDialog msg(Manager::Get()->GetAppWindow(),
							_("The new class has been created."),
							_("Information"),
							wxOK | wxICON_INFORMATION);
			msg.ShowModal();
			return 0;
		}
		wxMessageDialog msg(Manager::Get()->GetAppWindow(),
							_("The new class has been created.\n"
							"Do you want to add it to the current project?"),
							_("Add to project?"),
							wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
		if (msg.ShowModal() == wxID_YES)
		{
			int target = prjMan->AddFileToProject(dlg.GetHeaderFilename(), prj, -1);
			if (target != -1)
				prjMan->AddFileToProject(dlg.GetImplementationFilename(), prj, target);
			prjMan->RebuildTree();
		}
		return 0;
	}
		
	return -1;
}
