/*
* This file is part of lib_finder plugin for Code::Blocks Studio
* Copyright (C) 2006-2007  Bartlomiej Swiecki
*
* wxSmith is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* wxSmith is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmith; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
*
* $Revision: 4504 $
* $Id: wxsmithpluginregistrants.cpp 4504 2007-10-02 21:52:30Z byo $
* $HeadURL: svn+ssh://byo@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/plugin/wxsmithpluginregistrants.cpp $
*/

#ifndef RESULTMAP_H
#define RESULTMAP_H

#include <wx/dynarray.h>
#include <wx/hashmap.h>

#include "libraryresult.h"

class wxArrayString;
class wxString;

WX_DEFINE_ARRAY(LibraryResult*,ResultArray);

class ResultMap
{
    public:
        ResultMap();
        virtual ~ResultMap();

        /** \brief Clearing all results */
        void Clear();

        /** \brief Getting array associated with specified variable name */
        ResultArray& GetGlobalVar(const wxString& Name) { return Map[Name]; }

        /** \brief Checking if given global variable does exist */
        bool IsGlobalVar(const wxString& Name);

        /** \brief Getting all results */
        void GetAllResults(ResultArray& Array);

        /** \brief Getting array of used variable names */
        void GetGlobalVarNames(wxArrayString& Names);

    private:

        WX_DECLARE_STRING_HASH_MAP(ResultArray,ResultHashMap);
        ResultHashMap Map;
};

#endif
