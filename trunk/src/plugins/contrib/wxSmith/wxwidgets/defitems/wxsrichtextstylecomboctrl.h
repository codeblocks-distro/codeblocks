/** \file wxsrichtextstylecomboctrl.h
*
* This file is part of wxSmith plugin for Code::Blocks Studio
* Copyright (C) 2010 Gary Harris
*
* wxSmith is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* wxSmith is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmith. If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef WXSRICHTEXTSTYLECOMBOCTRL_H
#define WXSRICHTEXTSTYLECOMBOCTRL_H

#include "../wxswidget.h"

/** \brief Class for wxsRichTextStyleComboCtrl widget */
class wxsRichTextStyleComboCtrl: public wxsWidget
{
    public:

        wxsRichTextStyleComboCtrl(wxsItemResData* Data);

    private:

        virtual void OnBuildCreatingCode();
        virtual wxObject* OnBuildPreview(wxWindow* Parent,long Flags);
        virtual void OnEnumWidgetProperties(long Flags);

		wxString	m_sControl;
		wxString	m_sStyleSheet;
};

#endif
