#include "wxsheaders.h"
#include "wxswizard.h"

#include <wx/filename.h>
#include <cbproject.h>
#include <projectmanager.h>
#include <configmanager.h>
#include <manager.h>
#include <messagemanager.h>
#include "wxsmith.h"

/* ************************************************************************** */
/*  Flags for source file parts                                               */
/* ************************************************************************** */

static const int wxsSrcAny = 0x00;
static const int wxsSrcXrc = 0x01;
static const int wxsSrcSrc = 0x02;
static const int wxsSrcDsc = 0x04;
static const int wxsSrcMnu = 0x08;
static const int wxsSrcSta = 0x10;

struct wxsFilePart
{
    const wxChar* Text;
    int Flags;
};

/* ************************************************************************** */
/*  Source code for "app.h" file                                              */
/* ************************************************************************** */

static const wxChar wxsFileAppH_1[] =
_T("#ifndef APP_H\n")
_T("#define APP_H\n")
_T("\n")
_T("#include <wx/wxprec.h>\n")
_T("\n")
_T("#ifdef __BORLANDC__\n")
_T("\t#pragma hdrstop\n")
_T("#endif\n")
_T("\n")
_T("#ifndef WX_PRECOMP\n")
_T("\t#include <wx/wx.h>\n")
_T("#endif\n")
_T("\n")
_T("class MyApp : public wxApp\n")
_T("{\n")
_T("\tpublic:\n")
_T("\t\tvirtual bool OnInit();\n")
_T("};\n")
_T("\n")
_T("#endif // APP_H\n");

static wxsFilePart wxsFileAppH[] =
{
    { wxsFileAppH_1, 0 },
    { NULL }
};

/* ************************************************************************** */
/*  Source code for "app.cpp" file                                            */
/* ************************************************************************** */

static const wxChar wxsFileAppCpp_1[] =
_T("#include \"app.h\"\n")
_T("\n")
_T("//(*AppHeaders\n")
_T("#include \"mainframe.h\"\n");

static const wxChar wxsFileAppCpp_2_xrc[] =
_T("#include <wx/xrc/xmlres.h>\n");

static const wxChar wxsFileAppCpp_3[] =
_T("//*)\n")
_T("\n")
_T("IMPLEMENT_APP(MyApp);\n")
_T("\n")
_T("bool MyApp::OnInit()\n")
_T("{\n")
_T("\n")
_T("\t//(*AppInitialize\n")
_T("\tbool wxsOK = true;\n");

static const wxChar wxsFileAppCpp_4_xrc[] =
_T("\twxXmlResource::Get()->InitAllHandlers();\n")
_T("wxsOK = wxsOK && wxXmlResource::Get()->Load(_T(\"mainframe.xrc\"));\n");

static const wxChar wxsFileAppCpp_5[] =
_T("\tif ( wxsOK )\n")
_T("\t{\n")
_T("\t\tMainFrame* MainResource = new MainFrame(0L);\n")
_T("\t\tif ( MainResource ) MainResource->Show();\n")
_T("\t}\n")
_T("\t//*)\n")
_T("\n")
_T("\treturn wxsOK;\n")
_T("}\n");

static wxsFilePart wxsFileAppCpp[] =
{
    { wxsFileAppCpp_1, 0 },
    { wxsFileAppCpp_2_xrc, wxsSrcXrc },
    { wxsFileAppCpp_3, 0 },
    { wxsFileAppCpp_4_xrc, wxsSrcXrc },
    { wxsFileAppCpp_5, 0 },
    { NULL }
};

/* ************************************************************************** */
/*  Source code for "wx_pch.h"                                                */
/* ************************************************************************** */

static const wxChar wxsWxPchH_1[] =
_T("#ifndef WX_PCH_H_INCLUDED\n")
_T("#define WX_PCH_H_INCLUDED\n")
_T("\n")
_T("#if ( defined(USE_PCH) && !defined(WX_PRECOMP) )\n")
_T("\t#define WX_PRECOMP\n")
_T("#endif // USE_PCH\n")
_T("\n")
_T("// basic wxWidgets headers\n")
_T("#include <wx/wxprec.h>\n")
_T("\n")
_T("#ifdef __BORLANDC__\n")
_T("\t#pragma hdrstop\n")
_T("#endif\n")
_T("\n")
_T("#ifndef WX_PRECOMP\n")
_T("\t#include <wx/wx.h>\n")
_T("#endif\n")
_T("\n")
_T("#ifdef USE_PCH\n")
_T("\t// put here all your rarely-changing header files\n")
_T("#endif // USE_PCH\n")
_T("\n")
_T("#endif // WX_PCH_H_INCLUDED\n");

static wxsFilePart wxsWxPchH[] =
{
    { wxsWxPchH_1, 0 },
    { NULL }
};

/* ************************************************************************** */
/*  Content of "mainframe.h"                                                  */
/* ************************************************************************** */

static const wxChar wxsMainFrameH_1[] =
_T("#ifndef MAINFRAME_H\n")
_T("#define MAINFRAME_H\n")
_T("\n");

static const wxChar wxsMainFrameH_2_dsc[] =
_T("/* ********************************************************************** */\n")
_T("/*  wxSmith will automatically generate list of include files required    */\n")
_T("/*  by this resource. All includes between \"//(*Headers...\" and \"//*)\"    */\n")
_T("/*  comments are generated by wxSmith and will refresh automatically.     */\n")
_T("/* ********************************************************************** */\n");

static const wxChar wxsMainFrameH_3[] =
_T("\n")
_T("//(*Headers(MainFrame)\n")
_T("#include <wx/frame.h>\n")
_T("#include <wx/intl.h>\n")
_T("#include <wx/sizer.h>\n")
_T("#include <wx/stattext.h>\n")
_T("#include <wx/xrc/xmlres.h>\n")
_T("//*)\n")
_T("\n")
_T("class MainFrame: public wxFrame\n")
_T("{\n")
_T("\tpublic:\n")
_T("\n")
_T("\t\tMainFrame(wxWindow* parent,wxWindowID id = -1);\n")
_T("\t\tvirtual ~MainFrame();\n")
_T("\n");

static const wxChar wxsMainFrameH_4_dsc[] =
_T("\t\t/* ****************************************************************** */\n")
_T("\t\t/*  Enum declared below is list of identifiers generated by wxSmith.  */\n")
_T("\t\t/*  This list is automatically refreshed.                             */\n")
_T("\t\t/* ****************************************************************** */\n");

static const wxChar wxsMainFrameH_5[] =
_T("\n")
_T("\t\t//(*Identifiers(MainFrame)\n");

static const wxChar wxsMainFrameH_6_src[] =
_T("\t\tenum Identifiers\n")
_T("\t\t{\n")
_T("\t\t\tID_STATICTEXT1 = 0x1000\n")
_T("\t\t};\n");

static const wxChar wxsMainFrameH_7[] =
_T("\t\t//*)\n")
_T("\n")
_T("\tprotected:\n")
_T("\n");

static const wxChar wxsMainFrameH_8_dsc[] =
_T("\t\t/* ****************************************************************** */\n")
_T("\t\t/*  In next part of code (between \"//(*Handlers...\" and \"//*)\"        */\n")
_T("\t\t/*  comments contains declarations of event handlers. This is the     */\n")
_T("\t\t/*  only one part of code manager by wxSmith where user can add it's  */\n")
_T("\t\t/*  own code (but only event handlers are acceptable).                */\n")
_T("\t\t/* ****************************************************************** */\n");

static const wxChar wxsMainFrameH_9[] =
_T("\n")
_T("\t\t//(*Handlers(MainFrame)\n")
_T("\t\tvoid OnQuit(wxCommandEvent& event);\n")
_T("\t\tvoid OnAbout(wxCommandEvent& event);\n")
_T("\t\t//*)\n")
_T("\n");

static const wxChar wxsMainFrameH_10_dsc[] =
_T("\t\t/* ****************************************************************** */\n")
_T("\t\t/*  Declarations below are pointers to window components which have   */\n")
_T("\t\t/*  \"Is Member\" property set to true. These pointers can be used\t  */\n")
_T("\t\t/*  anywhere in this class (and even outside) just after the class is */\n")
_T("\t\t/*  created.                                                          */\n")
_T("\t\t/* ****************************************************************** */\n");

static const wxChar wxsMainFrameH_11[] =
_T("\n")
_T("\t\t//(*Declarations(MainFrame)\n")
_T("\t\twxStaticText* StaticText1;\n")
_T("\t\t//*)\n")
_T("\n")
_T("\tprivate:\n")
_T("\n")
_T("\t\tDECLARE_EVENT_TABLE()\n")
_T("};\n")
_T("\n")
_T("#endif\n");

static wxsFilePart wxsMainFrameH[] =
{
    { wxsMainFrameH_1, 0 },
    { wxsMainFrameH_2_dsc, wxsSrcDsc },
    { wxsMainFrameH_3, 0 },
    { wxsMainFrameH_4_dsc, wxsSrcDsc },
    { wxsMainFrameH_5, 0 },
    { wxsMainFrameH_6_src, wxsSrcSrc },
    { wxsMainFrameH_7, 0 },
    { wxsMainFrameH_8_dsc, wxsSrcDsc },
    { wxsMainFrameH_9, 0 },
    { wxsMainFrameH_10_dsc, wxsSrcDsc },
    { wxsMainFrameH_11, 0 },
    { NULL }
};

/* ************************************************************************** */
/*  Source file "mainframe.cpp"                                               */
/* ************************************************************************** */

static const wxChar wxsMainFrameCpp_1[] =
_T("#include \"mainframe.h\"\n")
_T("\n")
_T("#include <wx/settings.h>\n")
_T("#include <wx/menu.h>\n")
_T("#include <wx/statusbr.h>\n")
_T("#include <wx/msgdlg.h>\n")
_T("\n")
_T("int idMenuQuit = wxNewId();\n")
_T("int idMenuAbout = wxNewId();\n")
_T("\n");

static const wxChar wxsMainFrameCpp_2_dsc[] =
_T("/* ********************************************************************** */\n")
_T("/*  wxSmith is able to manage event tables. Currently it supports events  */\n")
_T("/*  generated by widgets. If You waht to add Your own event handler, You  */\n")
_T("/*  can add it before or after code block between \"//(*EventTable...\" and */\n")
_T("/*  \"//*)\" comments.                                                      */\n")
_T("/* ********************************************************************** */\n");

static const wxChar wxsMainFrameCpp_3[] =
_T("\n")
_T("BEGIN_EVENT_TABLE(MainFrame,wxFrame)\n")
_T("\t//(*EventTable(MainFrame)\n")
_T("\t//*)\n")
_T("\tEVT_MENU(idMenuQuit, MainFrame::OnQuit)\n")
_T("\tEVT_MENU(idMenuAbout, MainFrame::OnAbout)\n")
_T("END_EVENT_TABLE()\n")
_T("\n")
_T("MainFrame::MainFrame(wxWindow* parent,wxWindowID id)\n")
_T("{\n");

static const wxChar wxsMainFrameCpp_4_dsc[] =
_T("\t/* ********************************************************************** */\n")
_T("\t/*  The part below (up to \"//*)\" comment ) is automatically generated by  */\n")
_T("\t/*  wxSmith. This part is used to set-up resource (in this case it is     */\n")
_T("\t/*  MainFrame class). One shouldn't add anything here because all changed */\n")
_T("\t/*  will be lost while next code refresh made by wxSmith.                 */\n")
_T("\t/* ********************************************************************** */\n");

static const wxChar wxsMainFrameCpp_5_src[] =
_T("\n")
_T("\t//(*Initialize(MainFrame)\n")
_T("\twxBoxSizer* BoxSizer1;\n")
_T("\n")
_T("\tCreate(parent,id,_(\"wxSmith template\"),wxDefaultPosition,wxDefaultSize,wxCAPTION|wxTHICK_FRAME|wxSYSTEM_MENU|wxRESIZE_BOX|wxCLOSE_BOX|wxMINIMIZE_BOX);\n")
_T("\tSetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));\n")
_T("\tBoxSizer1 = new wxBoxSizer(wxHORIZONTAL);\n")
_T("\tStaticText1 = new wxStaticText(this,ID_STATICTEXT1,_(\"Hello world !!!\\n==============\\n\\nThis is template for wxWidgets application using wxSmith RAD gui editor\\nYou can add new resources through wxSmith menu.\\nAll resources managed inside wxSmith are listed in Resources tab.\\n\\n\"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE);\n")
_T("\tStaticText1->SetFont(wxFont(10,wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T(\"\")));\n")
_T("\tBoxSizer1->Add(StaticText1,1,wxALL|wxALIGN_CENTER,5);\n")
_T("\tthis->SetSizer(BoxSizer1);\n")
_T("\tBoxSizer1->Fit(this);\n")
_T("\tBoxSizer1->SetSizeHints(this);\n")
_T("\tCenter();\n")
_T("\t//*)\n")
_T("\n");

static const wxChar wxsMainFrameCpp_5_xrc[] =
_T("\n")
_T("\t//(*Initialize(MainFrame)\n")
_T("\twxXmlResource::Get()->LoadFrame(this,parent,_T(\"MainFrame\"));\n")
_T("\tStaticText1 = XRCCTRL(*this,\"ID_STATICTEXT1\",wxStaticText);\n")
_T("\t//*)\n")
_T("\n");

static const wxChar wxsMainFrameCpp_6_dsc[] =
_T("\t/* ********************************************************************** */\n")
_T("\t/*  Here You can put additional anynthinig else You want to be done       */\n")
_T("\t/*  during frame initialization. Because wxSmith doesn't support (yet)    */\n")
_T("\t/*  menus and status bars, we will add it them by hand.                   */\n")
_T("\t/* ********************************************************************** */\n");

static const wxChar wxsMainFrameCpp_7_menu[] =
_T("\n")
_T("\twxMenuBar* mbar = new wxMenuBar();\n")
_T("\twxMenu* fileMenu = new wxMenu(_(\"\"));\n")
_T("\tfileMenu->Append(idMenuQuit, _(\"&Quit\\tAlt-F4\"), _(\"Quit the application\"));\n")
_T("\tmbar->Append(fileMenu, _(\"&File\"));\n")
_T("\n")
_T("\twxMenu* helpMenu = new wxMenu(_(\"\"));\n")
_T("\thelpMenu->Append(idMenuAbout, _(\"&About\\tF1\"), _(\"Show info about this application\"));\n")
_T("\tmbar->Append(helpMenu, _(\"&Help\"));\n")
_T("\n")
_T("\tSetMenuBar(mbar);\n");

static const wxChar wxsMainFrameCpp_8_status[] =
_T("\n")
_T("\tnew wxStatusBar(this,-1);\n");

static const wxChar wxsMainFrameCpp_9[] =
_T("}\n")
_T("\n")
_T("MainFrame::~MainFrame()\n")
_T("{\n")
_T("}\n")
_T("\n")
_T("void MainFrame::OnQuit(wxCommandEvent& event)\n")
_T("{\n")
_T("\tClose();\n")
_T("}\n")
_T("\n")
_T("void MainFrame::OnAbout(wxCommandEvent& event)\n")
_T("{\n")
_T("\twxMessageBox(_(\"wxWidgets Application Template\"), _(\"Welcome to...\"));\n")
_T("}\n");

static const wxsFilePart wxsMainFrameCpp[] =
{
    { wxsMainFrameCpp_1, 0 },
    { wxsMainFrameCpp_2_dsc, wxsSrcDsc },
    { wxsMainFrameCpp_3, 0 },
    { wxsMainFrameCpp_4_dsc, wxsSrcDsc },
    { wxsMainFrameCpp_5_src, wxsSrcSrc },
    { wxsMainFrameCpp_5_xrc, wxsSrcXrc },
    { wxsMainFrameCpp_6_dsc, wxsSrcDsc },
    { wxsMainFrameCpp_7_menu, wxsSrcMnu },
    { wxsMainFrameCpp_8_status, wxsSrcSta },
    { wxsMainFrameCpp_9, 0 },
    { NULL }
};

/* ************************************************************************** */
/*  Configuration file "wxsmith.cfg"                                          */
/* ************************************************************************** */

static const wxChar wxsWxSmithCfg_1_src[] =
_T("<wxsmith>\n")
_T("\t<frame wxs_file=\"MainFrame.wxs\" class=\"MainFrame\" src_file=\"mainframe.cpp\" header_file=\"mainframe.h\" xrc_file=\"\" edit_mode=\"Source\" />\n")
//_T("\t<dialog wxs_file=\"AboutDlg.wxs\" class=\"AboutDlg\" src_file=\"aboutdlg.cpp\" header_file=\"aboutdlg.h\" xrc_file=\"\" edit_mode=\"Source\" />\n")
_T("\t<configuration app_src_file=\"app.cpp\" main_resource=\"MainFrame\" init_all_handlers=\"necessary\" />\n")
_T("</wxsmith>\n");

static const wxChar wxsWxSmithCfg_1_xrc[] =
_T("<wxsmith>\n")
_T("\t<frame wxs_file=\"MainFrame.wxs\" class=\"MainFrame\" src_file=\"mainframe.cpp\" header_file=\"mainframe.h\" xrc_file=\"mainframe.xrc\" edit_mode=\"Xrc\" />\n")
//_T("\t<dialog wxs_file=\"AboutDlg.wxs\" class=\"AboutDlg\" src_file=\"aboutdlg.cpp\" header_file=\"aboutdlg.h\" xrc_file=\"aboutdlg.xrc\" edit_mode=\"Source\" />\n")
_T("\t<configuration app_src_file=\"app.cpp\" main_resource=\"MainFrame\" init_all_handlers=\"necessary\" />\n")
_T("\t<load_resource>mainframe.xrc</load_resource>\n")
_T("</wxsmith>\n");

static wxsFilePart wxsWxSmithCfg[] =
{
    { wxsWxSmithCfg_1_src, wxsSrcSrc },
    { wxsWxSmithCfg_1_xrc, wxsSrcXrc },
    { NULL }
};

/* ************************************************************************** */
/*  Content of resource file "MainFrame.wxs"                                  */
/* ************************************************************************** */

const wxChar wxsMainFrameWxs_1[] =
_T("<resource>\n")
_T("    <object class=\"wxFrame\" name=\"MainFrame\">\n")
_T("        <title>wxSmith template</title>\n")
_T("        <centered>1</centered>\n")
_T("        <style>wxCAPTION|wxTHICK_FRAME|wxSYSTEM_MENU|wxRESIZE_BOX|wxCLOSE_BOX|wxMINIMIZE_BOX</style>\n")
_T("        <bg>wxSYS_COLOUR_BTNFACE</bg>\n")
_T("        <object class=\"wxBoxSizer\" variable=\"BoxSizer1\" member=\"no\">\n")
_T("            <orient>wxHORIZONTAL</orient>\n")
_T("            <object class=\"sizeritem\">\n")
_T("                <option>1</option>\n")
_T("                <border>5</border>\n")
_T("                <flag>wxALL|wxALIGN_CENTER</flag>\n")
_T("                <object class=\"wxStaticText\" name=\"ID_STATICTEXT1\" variable=\"StaticText1\" member=\"yes\">\n")
_T("                    <label>Hello world !!!&#x0A;==============&#x0A;&#x0A;This is template for wxWidgets application using wxSmith RAD gui editor&#x0A;You can add new resources through wxSmith menu.&#x0A;All resources managed inside wxSmith are listed in Resources tab.&#x0A;&#x0A;</label>\n")
_T("                    <style>wxALIGN_CENTRE</style>\n")
_T("                    <font>\n")
_T("                        <size>10</size>\n")
_T("                        <weight>bold</weight>\n")
_T("                    </font>\n")
_T("                </object>\n")
_T("            </object>\n")
_T("        </object>\n")
_T("    </object>\n")
_T("</resource>\n");

const wxsFilePart wxsMainFrameWxs[] =
{
    { wxsMainFrameWxs_1, 0 },
    { NULL }
};


/* ************************************************************************** */
/*  Content of mainframe.xrc file                                             */
/* ************************************************************************** */

static const wxChar wxsMainFrameXrc_1[] =
_T("<resource>\n")
_T("    <object class=\"wxFrame\" name=\"MainFrame\">\n")
_T("        <title>wxSmith template</title>\n")
_T("        <centered>1</centered>\n")
_T("        <style>wxCAPTION|wxTHICK_FRAME|wxSYSTEM_MENU|wxRESIZE_BOX|wxCLOSE_BOX|wxMINIMIZE_BOX</style>\n")
_T("        <bg>wxSYS_COLOUR_BTNFACE</bg>\n")
_T("        <object class=\"wxBoxSizer\">\n")
_T("            <orient>wxHORIZONTAL</orient>\n")
_T("            <object class=\"sizeritem\">\n")
_T("                <option>1</option>\n")
_T("                <border>5</border>\n")
_T("                <flag>wxALL|wxALIGN_CENTER</flag>\n")
_T("                <object class=\"wxStaticText\" name=\"ID_STATICTEXT1\">\n")
_T("                    <label>Hello world !!!&#x0A;==============&#x0A;&#x0A;This is template for wxWidgets application using wxSmith RAD gui editor&#x0A;You can add new resources through wxSmith menu.&#x0A;All resources managed inside wxSmith are listed in Resources tab.&#x0A;&#x0A;</label>\n")
_T("                    <pos>-1,-1</pos>\n")
_T("                    <style>wxALIGN_CENTRE</style>\n")
_T("                    <font>\n")
_T("                        <size>10</size>\n")
_T("                        <weight>bold</weight>\n")
_T("                        <face>MS Shell Dlg 2</face>\n")
_T("                    </font>\n")
_T("                </object>\n")
_T("            </object>\n")
_T("        </object>\n")
_T("    </object>\n")
_T("</resource>\n");

static wxsFilePart wxsMainFrameXrc[] =
{
    { wxsMainFrameXrc_1, wxsSrcXrc },
    { NULL }
};

BEGIN_EVENT_TABLE(wxsWizard,wxDialog)
	//(*EventTable(wxsWizard)
	EVT_TEXT(ID_TEXTCTRL1,wxsWizard::OnPrjNameText)
	EVT_TEXT(ID_TEXTCTRL6,wxsWizard::OnBaseDirText)
	EVT_BUTTON(ID_BUTTON5,wxsWizard::OnBaseDirChooseClick)
	EVT_CHECKBOX(ID_CHECKBOX6,wxsWizard::OnUseCustomPrjDirChange)
	EVT_BUTTON(ID_BUTTON3,wxsWizard::OnDirChooseClick)
	EVT_COMBOBOX(ID_COMBOBOX2,wxsWizard::OnConfModeSelect)
	EVT_BUTTON(ID_BUTTON4,wxsWizard::OnwxDirChooseClick)
	EVT_BUTTON(ID_BUTTON1,wxsWizard::OnButton1Click)
	EVT_BUTTON(ID_BUTTON2,wxsWizard::OnButton2Click)
	//*)
END_EVENT_TABLE()

wxsWizard::wxsWizard(wxWindow* parent,wxWindowID id):
    Initialized(false)
{
	//(*Initialize(wxsWizard)
	wxFlexGridSizer* FlexGridSizer2;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxFlexGridSizer* FlexGridSizer3;
	wxStaticText* StaticText1;
	wxStaticText* StaticText2;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxFlexGridSizer* FlexGridSizer4;
	wxStaticText* StaticText5;
	wxStaticText* StaticText4;
	wxStaticText* StaticText6;
	wxBoxSizer* BoxSizer4;
	wxStaticText* StaticText7;
	wxBoxSizer* BoxSizer1;
	wxButton* Button1;
	wxButton* Button2;

	Create(parent,id,_("wxSmith project wizzard"),wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE);
	MainSizer = new wxFlexGridSizer(0,1,0,0);
	FlexGridSizer2 = new wxFlexGridSizer(0,1,0,0);
	FlexGridSizer2->AddGrowableCol(0);
	FlexGridSizer2->AddGrowableRow(0);
	FlexGridSizer2->AddGrowableRow(1);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL,this,_("wxSmith project options"));
	FlexGridSizer3 = new wxFlexGridSizer(0,2,0,0);
	FlexGridSizer3->AddGrowableCol(1);
	StaticText1 = new wxStaticText(this,ID_STATICTEXT1,_("Project name:"),wxDefaultPosition,wxDefaultSize,0);
	PrjName = new wxTextCtrl(this,ID_TEXTCTRL1,_("wxSmith project"),wxDefaultPosition,wxDefaultSize,0);
	if ( 0 ) PrjName->SetMaxLength(0);
	StaticText2 = new wxStaticText(this,ID_STATICTEXT2,_("Main frame title:"),wxDefaultPosition,wxDefaultSize,0);
	FrmTitle = new wxTextCtrl(this,ID_TEXTCTRL2,_("Welcome to wxSmith application"),wxDefaultPosition,wxDefaultSize,0);
	if ( 0 ) FrmTitle->SetMaxLength(0);
	StaticText8 = new wxStaticText(this,ID_STATICTEXT8,_("Base directory:"),wxDefaultPosition,wxDefaultSize,0);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	BaseDir = new wxTextCtrl(this,ID_TEXTCTRL6,_T(""),wxDefaultPosition,wxDefaultSize,0);
	if ( 0 ) BaseDir->SetMaxLength(0);
	BaseDirChoose = new wxButton(this,ID_BUTTON5,_("..."),wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT);
	if (false) BaseDirChoose->SetDefault();
	BoxSizer5->Add(BaseDir,1,wxLEFT|wxBOTTOM|wxALIGN_CENTER,5);
	BoxSizer5->Add(BaseDirChoose,0,wxRIGHT|wxBOTTOM|wxALIGN_CENTER,5);
	UseCustomPrjDir = new wxCheckBox(this,ID_CHECKBOX6,_("Custom proj. dir:"),wxDefaultPosition,wxDefaultSize,0);
	UseCustomPrjDir->SetValue(false);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	PrjDir = new wxTextCtrl(this,ID_TEXTCTRL3,_T(""),wxDefaultPosition,wxDefaultSize,0);
	if ( 0 ) PrjDir->SetMaxLength(0);
	PrjDir->Disable();
	PrjDirChoose = new wxButton(this,ID_BUTTON3,_("..."),wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT);
	if (false) PrjDirChoose->SetDefault();
	PrjDirChoose->Disable();
	BoxSizer2->Add(PrjDir,1,wxLEFT|wxBOTTOM|wxALIGN_CENTER,5);
	BoxSizer2->Add(PrjDirChoose,0,wxRIGHT|wxBOTTOM|wxALIGN_CENTER,5);
	FlexGridSizer3->Add(StaticText1,1,wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL,5);
	FlexGridSizer3->Add(PrjName,1,wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_CENTER|wxEXPAND,5);
	FlexGridSizer3->Add(StaticText2,1,wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL,5);
	FlexGridSizer3->Add(FrmTitle,1,wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_CENTER|wxEXPAND,5);
	FlexGridSizer3->Add(StaticText8,1,wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL,5);
	FlexGridSizer3->Add(BoxSizer5,1,wxALIGN_CENTER|wxEXPAND,0);
	FlexGridSizer3->Add(UseCustomPrjDir,1,wxALL|wxALIGN_CENTER,5);
	FlexGridSizer3->Add(BoxSizer2,1,wxALIGN_CENTER|wxEXPAND,0);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	AddMenu = new wxCheckBox(this,ID_CHECKBOX1,_("Add simple menu"),wxDefaultPosition,wxDefaultSize,0);
	AddMenu->SetValue(true);
	AddStatus = new wxCheckBox(this,ID_CHECKBOX2,_("Add status bar"),wxDefaultPosition,wxDefaultSize,0);
	AddStatus->SetValue(true);
	AddAbout = new wxCheckBox(this,ID_CHECKBOX3,_("Create About dialog"),wxDefaultPosition,wxDefaultSize,0);
	AddAbout->SetValue(false);
	AddAbout->Hide();
	UseXrc = new wxCheckBox(this,ID_CHECKBOX4,_("Use XRC files"),wxDefaultPosition,wxDefaultSize,0);
	UseXrc->SetValue(false);
	AddDesc = new wxCheckBox(this,ID_CHECKBOX5,_("Insert additional comments describing wxSmith\'s code"),wxDefaultPosition,wxDefaultSize,0);
	AddDesc->SetValue(false);
	BoxSizer3->Add(AddMenu,1,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL,5);
	BoxSizer3->Add(AddStatus,1,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL,5);
	BoxSizer3->Add(AddAbout,1,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL,5);
	BoxSizer3->Add(UseXrc,1,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL,5);
	BoxSizer3->Add(AddDesc,1,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL,5);
	StaticBoxSizer1->Add(FlexGridSizer3,1,wxALIGN_CENTER|wxEXPAND,5);
	StaticBoxSizer1->Add(BoxSizer3,0,wxTOP|wxBOTTOM|wxALIGN_CENTER|wxEXPAND,5);
	wxWidgetsConfig = new wxStaticBoxSizer(wxVERTICAL,this,_("wxWigets library options"));
	FlexGridSizer4 = new wxFlexGridSizer(0,2,0,0);
	FlexGridSizer4->AddGrowableCol(1);
	StaticText5 = new wxStaticText(this,ID_STATICTEXT5,_("Configuration mode:"),wxDefaultPosition,wxDefaultSize,0);
	ConfMode = new wxComboBox(this,ID_COMBOBOX2,_T(""),wxDefaultPosition,wxDefaultSize,0,NULL,0);
	ConfMode->Append(_("Global variables"));
	ConfMode->Append(_("Custom variables"));
	ConfMode->Append(_("Library finder"));
	ConfMode->SetSelection(0);
	StaticText4 = new wxStaticText(this,ID_STATICTEXT4,_("Library type:"),wxDefaultPosition,wxDefaultSize,0);
	LibType = new wxComboBox(this,ID_COMBOBOX1,_T(""),wxDefaultPosition,wxDefaultSize,0,NULL,0);
	LibType->Append(_("DLL"));
	LibType->Append(_("Static"));
	LibType->SetSelection(0);
	StaticText6 = new wxStaticText(this,ID_STATICTEXT6,_("wxWidgest dir:"),wxDefaultPosition,wxDefaultSize,0);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	WxDir = new wxTextCtrl(this,ID_TEXTCTRL4,_T(""),wxDefaultPosition,wxDefaultSize,0);
	if ( 0 ) WxDir->SetMaxLength(0);
	WxDir->Disable();
	WxDirChoose = new wxButton(this,ID_BUTTON4,_("..."),wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT);
	if (false) WxDirChoose->SetDefault();
	WxDirChoose->Disable();
	BoxSizer4->Add(WxDir,1,wxTOP|wxALIGN_CENTER|wxEXPAND,5);
	BoxSizer4->Add(WxDirChoose,0,wxTOP|wxALIGN_CENTER,5);
	StaticText7 = new wxStaticText(this,ID_STATICTEXT7,_("Configuration:"),wxDefaultPosition,wxDefaultSize,0);
	WxConf = new wxTextCtrl(this,ID_TEXTCTRL5,_T(""),wxDefaultPosition,wxDefaultSize,0);
	if ( 0 ) WxConf->SetMaxLength(0);
	WxConf->Disable();
	FlexGridSizer4->Add(StaticText5,1,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL,5);
	FlexGridSizer4->Add(ConfMode,1,wxLEFT|wxRIGHT|wxALIGN_CENTER|wxEXPAND,5);
	FlexGridSizer4->Add(StaticText4,1,wxLEFT|wxRIGHT|wxTOP|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL,5);
	FlexGridSizer4->Add(LibType,1,wxLEFT|wxRIGHT|wxTOP|wxALIGN_CENTER|wxEXPAND,5);
	FlexGridSizer4->Add(StaticText6,1,wxLEFT|wxRIGHT|wxTOP|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL,5);
	FlexGridSizer4->Add(BoxSizer4,1,wxLEFT|wxRIGHT|wxALIGN_CENTER|wxEXPAND,5);
	FlexGridSizer4->Add(StaticText7,1,wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL,5);
	FlexGridSizer4->Add(WxConf,1,wxALL|wxALIGN_CENTER|wxEXPAND,5);
	wxWidgetsConfig->Add(FlexGridSizer4,1,wxALIGN_CENTER|wxEXPAND,0);
	FlexGridSizer2->Add(StaticBoxSizer1,1,wxALL|wxALIGN_CENTER|wxEXPAND,5);
	FlexGridSizer2->Add(335,0,1);
	FlexGridSizer2->Add(wxWidgetsConfig,1,wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_CENTER|wxEXPAND,5);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	Button1 = new wxButton(this,ID_BUTTON1,_("Cancel"),wxDefaultPosition,wxDefaultSize,0);
	if (false) Button1->SetDefault();
	Button2 = new wxButton(this,ID_BUTTON2,_("Create"),wxDefaultPosition,wxDefaultSize,0);
	if (false) Button2->SetDefault();
	BoxSizer1->Add(Button1,1,wxALL|wxALIGN_CENTER,5);
	BoxSizer1->Add(Button2,1,wxALL|wxALIGN_CENTER,5);
	MainSizer->Add(FlexGridSizer2,1,wxALIGN_CENTER|wxEXPAND,0);
	MainSizer->Add(BoxSizer1,1,wxALIGN_CENTER,0);
	this->SetSizer(MainSizer);
	MainSizer->Fit(this);
	MainSizer->SetSizeHints(this);
	Center();
	//*)

    #ifndef __WXMSW__
        MainSizer->Show(wxWidgetsConfig,false,true);
        MainSizer->Fit(this);
        MainSizer->SetSizeHints(this);
    #endif


    BaseDir->SetValue(
        Manager::Get()->GetConfigManager(_T("wxsmith"))->Read(_T("wizardbasepath"),_T("")));
    Initialized = true;
    RebuildPrjDir();
}

wxsWizard::~wxsWizard()
{
}

void wxsWizard::OnButton1Click(wxCommandEvent& event)
{
    EndModal(1);
}

void wxsWizard::OnButton2Click(wxCommandEvent& event)
{
    wxString Dir = PrjDir->GetValue();
    wxString Name = PrjName->GetValue();

    if ( Name.empty() )
    {
        wxMessageBox(_("Please enter project name"));
        return;
    }

    if ( Dir.empty() )
    {
        wxMessageBox(_("Please enter project directory"));
        return;
    }

    if ( !wxFileName(Dir).IsAbsolute() )
    {
        wxMessageBox(_("Project path must be absolute"));
        return;
    }

    if ( !wxFileName::Mkdir(Dir,0777,wxPATH_MKDIR_FULL ) )
    {
        wxMessageBox(_("Couldn't create main project directory"));
        return;
    }

    wxString ProjectFileName = Dir + wxFileName::GetPathSeparator() +
        GetProjectFileName() + _T(".cbp");

    cbProject* project = Manager::Get()->GetProjectManager()->NewProject(ProjectFileName);
    if ( !project )
    {
        wxMessageBox(_("Couldn't create new project file"));
        return;
    }

    bool addedVars = false;
    #ifdef __WXMSW__

        // Configuring paths
        switch ( ConfMode->GetSelection() )
        {
            case 0: // Global variables
                project->AddIncludeDir(_T("$(#WX.include)"));
                project->AddIncludeDir(_T("$(#WX.lib)\\gcc_dll\\msw"));
                project->AddIncludeDir(_T("$(#WX.lib)\\gcc_dll$(WX_CFG)\\msw"));
                project->AddIncludeDir(_T("$(#WX)\\contrib\\include"));
                project->AddLibDir(_T("$(#WX.lib)\\gcc_dll"));
                project->AddLibDir(_T("$(#WX.lib)\\gcc_dll\\msw"));
                project->AddLibDir(_T("$(#WX.lib)\\gcc_dll$(WX_CFG)"));
                project->AddResourceIncludeDir(_T("$(#WX.include)"));
                project->SetVar(_T("WX_CFG"),_T(""));
                addedVars = true;
                break;

            case 1: // Custom variables
                project->AddIncludeDir(_T("$(WX_DIR)\\include"));
                project->AddIncludeDir(_T("$(WX_DIR)\\lib\\gcc_dll\\msw"));
                project->AddIncludeDir(_T("$(WX_DIR)\\lib\\gcc_dll$(WX_CFG)\\msw"));
                project->AddIncludeDir(_T("$(WX_DIR)\\contrib\\include"));
                project->AddLibDir(_T("$(WX_DIR)\\lib\\gcc_dll"));
                project->AddLibDir(_T("$(WX_DIR)\\lib\\gcc_dll\\msw"));
                project->AddLibDir(_T("$(WX_DIR)\\lib\\gcc_dll$(WX_CFG)"));
                project->AddResourceIncludeDir(_T("$(WX_DIR)\\include"));
                project->SetVar(_T("WX_DIR"),_T("C:\\wxWidgets-2.6.2"));
                project->SetVar(_T("WX_CFG"),_T(""));
                addedVars = true;
                break;

            case 2:
                project->AddIncludeDir(_T("$(#WX.include"));
                project->AddLibDir(_T("$(#WX.lib"));
                project->AddIncludeDir(_T("$(#WX.obj"));
                break;
        }

        // Configuring project options
        project->AddCompilerOption(_T("-DUSE_PCH"));
        switch ( ConfMode->GetSelection() )
        {
            case 0:
            case 1:
                project->AddCompilerOption(_T("-pipe"));
                project->AddCompilerOption(_T("-mthreads"));
                project->AddCompilerOption(_T("-fmessage-length=0"));
                project->AddCompilerOption(_T("-fexceptions"));
                project->AddCompilerOption(_T("-include \"wx_pch.h\""));
                project->AddCompilerOption(_T("-Winvalid-pch"));
                project->AddCompilerOption(_T("-D__GNUWIN32__"));
                project->AddCompilerOption(_T("-D__WXMSW__"));
                project->AddCompilerOption(_T("-DHAVE_W32API_H"));

                project->AddLinkLib(_T("wxmsw26"));

                switch ( LibType->GetSelection() )
                {
                    case 0: // DLL
                        project->AddCompilerOption(_T("-DWXUSINGDLL"));
                        break;

                    case 1: // static
                        project->AddLinkLib(_T("winspool"));
                        project->AddLinkLib(_T("winmm"));
                        project->AddLinkLib(_T("shell32"));
                        project->AddLinkLib(_T("comctl32"));
                        project->AddLinkLib(_T("ctl3d32"));
                        project->AddLinkLib(_T("odbc32"));
                        project->AddLinkLib(_T("advapi32"));
                        project->AddLinkLib(_T("wsock32"));
                        project->AddLinkLib(_T("opengl32"));
                        project->AddLinkLib(_T("glu32"));
                        project->AddLinkLib(_T("ole32"));
                        project->AddLinkLib(_T("oleaut32"));
                        project->AddLinkLib(_T("uuid"));
                        break;
                }
                break;

            case 2:
                project->AddCompilerOption(_T("$(#WX.cflags)"));
                project->AddLinkerOption(_T("$(#WX.lflags)"));
                break;
        }

    #else
        project->AddCompilerOption(_T("`wx-config --cflags`"));
        project->AddLinkerOption(_T("`wx-config --libs`"));
    #endif

    int flags = 0;
    if ( UseXrc->GetValue() )    flags |= wxsSrcXrc;
    else                         flags |= wxsSrcSrc;
    if ( AddMenu->GetValue() )   flags |= wxsSrcMnu;
    if ( AddStatus->GetValue() ) flags |= wxsSrcSta;
    if ( AddDesc->GetValue() )   flags |= wxsSrcDsc;

    if (
        !BuildFile(project,Dir,_T("app.h"),wxsFileAppH,flags,true,false,false,50) ||
        !BuildFile(project,Dir,_T("app.cpp"),wxsFileAppCpp,flags,true,true,true,50) ||
        !BuildFile(project,Dir,_T("wx_pch.h"),wxsWxPchH,flags,true,true,false,0) ||
        !BuildFile(project,Dir,_T("mainframe.h"),wxsMainFrameH,flags,true,false,false,50) ||
        !BuildFile(project,Dir,_T("mainframe.cpp"),wxsMainFrameCpp,flags,true,true,true,50) ||
        !BuildFile(project,Dir,_T("wxsmith/wxsmith.cfg"),wxsWxSmithCfg,flags,false,false,false,50) ||
        !BuildFile(project,Dir,_T("wxsmith/MainFrame.wxs"),wxsMainFrameWxs,flags,true,false,false,50) ||
        !BuildFile(project,Dir,_T("mainframe.xrc"),wxsMainFrameXrc,flags,false,false,false,50)
        )
    {
        wxMessageBox(_("Error occured while creating files. Project may be corrupted."));
    }

    Manager::Get()->GetProjectManager()->RebuildTree();
    if ( wxsPLUGIN() )
    {
        wxsProject* wxsProj = wxsPLUGIN()->GetSmithProject(project);
        // Rebinding to load new resource configuration
        if ( wxsProj )
        {
            wxsProj->BindProject(project);
        }
    }
    project->Save();
    Manager::Get()->GetConfigManager(_T("wxsmith"))->Write(_T("wizardbasepath"),BaseDir->GetValue());
    if ( addedVars )
    {
        wxMessageBox(_("New project created. But You may need\nto adjust some custom vars in project options"));
    }
    EndModal(0);
}

void wxsWizard::OnConfModeSelect(wxCommandEvent& event)
{
    bool EnableWxConf = ( ConfMode->GetSelection() != 0 );

    WxDir->Enable(EnableWxConf);
    WxDirChoose->Enable(EnableWxConf);
    WxConf->Enable(EnableWxConf);
}

void wxsWizard::OnDirChooseClick(wxCommandEvent& event)
{
    wxString Dir = wxDirSelector(wxDirSelectorPromptStr,PrjDir->GetValue());
    if ( !Dir.empty() ) PrjDir->SetValue(Dir);
}

void wxsWizard::OnwxDirChooseClick(wxCommandEvent& event)
{
    wxString Dir = wxDirSelector();
    if ( !Dir.empty() ) WxDir->SetValue(Dir);
}

bool wxsWizard::BuildFile(
    cbProject* project,
    const wxString& RootPath,
    const wxString& FileName,
    const wxsFilePart* FP,
    int Flags,
    bool AddToProject,
    bool Compile,
    bool Link,
    unsigned short Weight)
{
    bool IsAnyPartUsed = false;
    for ( const wxsFilePart* _FP = FP; _FP->Text; _FP++ )
    {
        if ( ( _FP->Flags & Flags ) == _FP->Flags )
        {
            IsAnyPartUsed = true;
            break;
        }
    }
    if ( !IsAnyPartUsed ) return true;

    wxString FullPath = RootPath + wxFileName::GetPathSeparator() + FileName;
    wxFileName FN(FullPath);
    if ( !wxFileName::Mkdir(FN.GetPath(),0777,wxPATH_MKDIR_FULL) ) return false;

    wxFile Fl(FN.GetFullPath(),wxFile::write);
    if ( !Fl.IsOpened() ) return false;

    bool AllWritten = true;
    for ( ; FP->Text; FP++ )
    {
        if ( ( FP->Flags & Flags ) == FP->Flags )
        {
            if ( !Fl.Write(FP->Text) ) AllWritten = false;
        }
    }

    if ( !AddToProject ) return AllWritten;
    ProjectFile* file = project->AddFile(0,FileName,Compile,Link,Weight);
    if ( !file )
    {
        AllWritten = false;
    }
    else
    {
        // Need to overridee defaqult settings since project->Add
        // can clear these settings
        file->compile = Compile;
        file->link = Link;
    }
    return AllWritten;
}

void wxsWizard::OnUseCustomPrjDirChange(wxCommandEvent& event)
{
    bool Enable = UseCustomPrjDir->GetValue();
    PrjDir->Enable(Enable);
    PrjDirChoose->Enable(Enable);
    RebuildPrjDir();
}

void wxsWizard::RebuildPrjDir()
{
    if ( !Initialized ) return;
    if ( UseCustomPrjDir->GetValue() ) return;
    PrjDir->SetValue(
        BaseDir->GetValue() + wxFileName::GetPathSeparator() +
        GetProjectFileName() );
}

wxString wxsWizard::GetProjectFileName()
{
    wxString Name = PrjName->GetValue();
    wxString ProjectFileName;
    for ( size_t i = 0; i < Name.Len(); i++ )
    {
        wxChar ch = Name[i];
        if ( (ch >= _T('A') && ch <= _T('Z')) ||
             (ch >= _T('a') && ch <= _T('z')) ||
             (ch >= _T('0') && ch <= _T('9')) ||
             (ch == _T('_')) )
        {
            ProjectFileName.Append(ch);
        }
        else if ( ch == _T(' ') )
        {
            ProjectFileName.Append(_T('_'));
        }
    }
    return ProjectFileName;
}

void wxsWizard::OnBaseDirChooseClick(wxCommandEvent& event)
{
    wxString Dir = ::wxDirSelector(wxDirSelectorPromptStr,BaseDir->GetValue());
    if ( !Dir.empty() ) BaseDir->SetValue(Dir);
    RebuildPrjDir();
}

void wxsWizard::OnBaseDirText(wxCommandEvent& event)
{
    RebuildPrjDir();
}

void wxsWizard::OnPrjNameText(wxCommandEvent& event)
{
    RebuildPrjDir();
}
