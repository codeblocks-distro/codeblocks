/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

//(*Headers(Protocol)
#include <wx/dialog.h>
class wxTextCtrl;
class wxStaticText;
class wxBoxSizer;
class wxButton;
class wxStaticBoxSizer;
//*)

class wxCommandEvent;
class wxWindow;
class wxString;

class Protocol: public wxDialog
{
public:

  Protocol(wxWindow* parent,wxWindowID id = -1);

  void SetProtocol(const wxArrayString& Protocol);

  //(*Identifiers(Protocol)
  static const long ID_LBL_PROTOCOL;
  static const long ID_TXT_PROTOCOL;
  //*)

protected:

  //(*Handlers(Protocol)
  void OnBtnOKClick(wxCommandEvent& event);
  //*)

  //(*Declarations(Protocol)
  wxBoxSizer* sizMain;
  wxTextCtrl* m_Protocol;
  wxStaticBoxSizer* sizProtocol;
  wxStaticText* lblProtocol;
  wxButton* m_OK;
  //*)

private:

  DECLARE_EVENT_TABLE()
};

#endif // PROTOCOL_H
