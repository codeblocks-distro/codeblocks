#include "wxsstdmanager.h"

#include "wxsbutton.h"
#include "wxsframe.h"
#include "wxspanel.h"
#include "wxsdialog.h"
#include "wxsgridsizer.h"
#include "wxsstatictext.h"
#include "wxscheckbox.h"
#include "wxstogglebutton.h"
#include "wxscombobox.h"
#include "wxslistbox.h"

#include <wx/xrc/xmlres.h>
#include <configmanager.h>
#include <wx/fs_zip.h>

/******************************************************************************/
/* Infos for standard widgets                                                 */
/******************************************************************************/

static const char* DefLicence     = "wxWidgets License";
static const char* DefAuthor      = "wxWidgets Team";
static const char* DefAuthorEmail = "";
static const char* DefAuthorSite  = "www.wxwidgets.org";
static const char* DefCategory    = "Standard";


#define Entry(Name,Link)                                \
    {   "wx"#Name,                                      \
        DefLicence,                                     \
        DefAuthor,                                      \
        DefAuthorEmail,                                 \
        DefAuthorSite,                                  \
        "http://www.wxwidgets.org/manuals/2.6.1/"Link,  \
        DefCategory,                                    \
        false,                                          \
        false,                                          \
        2, 6,                                          \
        NULL,                                           \
        &wxsStdManager,                                 \
        wxs##Name##Id,                                  \
        0,                                              \
        wxs##Name##Styles                               \
    },

#define EntryNoStyles(Name,Link)                        \
    {   "wx"#Name,                                      \
        DefLicence,                                     \
        DefAuthor,                                      \
        DefAuthorEmail,                                 \
        DefAuthorSite,                                  \
        "http://www.wxwidgets.org/manuals/2.6.1/"Link,  \
        DefCategory,                                    \
        false,                                          \
        false,                                          \
        2, 6,                                           \
        NULL,                                           \
        &wxsStdManager,                                 \
        wxs##Name##Id,                                  \
        0,                                              \
        NULL                                            \
    },

#define WindowEntry(Name,Link)                          \
    {   "wx"#Name,                                      \
        DefLicence,                                     \
        DefAuthor,                                      \
        DefAuthorEmail,                                 \
        DefAuthorSite,                                  \
        "http://www.wxwidgets.org/manuals/2.6.1/"Link,  \
        "",                                             \
        false,                                          \
        false,                                          \
        2, 6,                                           \
        NULL,                                           \
        &wxsStdManager,                                 \
        wxs##Name##Id,                                  \
        0,                                              \
        wxs##Name##Styles                               \
    },


static wxsWidgetInfo StdInfos[] =
{
    { "", "", "", "", "", "", "", false, false, 0, 0, NULL, NULL, wxsNoneId },  // NONE
    
    {   "wxGridSizer",
        DefLicence,
        DefAuthor,
        DefAuthorEmail,
        DefAuthorSite,
        "http://www.wxwidgets.org/manuals/2.4.2/wx189.htm#wxgridsizer",
        DefCategory,
        true,
        true,
        2, 42,
        NULL,
        &wxsStdManager,
        wxsGridSizerId,
        0,
        NULL
    },
 
    Entry(Button,      "wx_wxbutton.htm#wxbutton")
    Entry(ToggleButton,"wx_wxtogglebutton.html#wxtogglebutton")
    Entry(CheckBox,    "wx_wxcheckbox.html#wxcheckbox")
    Entry(StaticText,  "wx_wxstatictext.html#wxstatictext")
    Entry(ComboBox,    "wx_wxcombobox.html#wxcombobox")
    Entry(ListBox,     "wx_wxlistbox.html#wxlistbox")    
    Entry(Panel,       "wx_wxpanel.html#wxpanel")    
    
    WindowEntry(Dialog,"wx_wxdialog.html#wxdialog")
    WindowEntry(Frame, "wx_wxframe.html#wxframe")
    WindowEntry(Panelr,"wx_wxpanel.html#wxpanel")
    
};

static const int StdInfosCnt = sizeof(StdInfos) / sizeof(StdInfos[0]);


wxsStdManagerT::wxsStdManagerT()
{
}

wxsStdManagerT::~wxsStdManagerT()
{
    for ( int i=0; i<StdInfosCnt; i++ )
    {
        if ( StdInfos[i].Icon )
        {
            delete StdInfos[i].Icon;
            StdInfos[i].Icon = NULL;
        }
    }
}

bool wxsStdManagerT::Initialize()
{
    wxString resPath = ConfigManager::Get()->Read("data_path", wxEmptyString);
    for ( int i=1; i<StdInfosCnt; i++ )
    {
        wxString FileName = resPath + wxT("/images/wxsmith/") + StdInfos[i].Name + wxT(".png");
        wxBitmap* Bmp = new wxBitmap;
        if ( wxFileName::FileExists(FileName) )
        {
            Bmp->LoadFile(FileName,wxBITMAP_TYPE_PNG);
                
            if ( Bmp->Ok() )
            {
                StdInfos[i].Icon = Bmp;
            }
            else
            {
                StdInfos[i].Icon = NULL;
                delete Bmp;
            }
        }
        else
        {
            StdInfos[i].Icon = NULL;
        }
    }
    return true;
}

int wxsStdManagerT::GetCount()
{
    return wxsStdIdCount;
}
        
/** Getting widget's info */
const wxsWidgetInfo* wxsStdManagerT::GetWidgetInfo(int Number)
{
    if ( Number < 0 || Number >= StdInfosCnt ) Number = wxsNoneId;
    assert ( StdInfos[Number].Id == Number );
    return &StdInfos[Number];
}

/** Getting new widget */
wxsWidget* wxsStdManagerT::ProduceWidget(int Id)
{
    switch ( Id )
    {
        case wxsGridSizerId:    return new wxsGridSizer(this);
        case wxsButtonId:       return new wxsButton(this);
        case wxsCheckBoxId:     return new wxsCheckBox(this);
        case wxsStaticTextId:   return new wxsStaticText(this);
        case wxsToggleButtonId: return new wxsToggleButton(this);
        case wxsComboBoxId:     return new wxsComboBox(this);
        case wxsListBoxId:      return new wxsListBox(this);
        case wxsPanelId:        return new wxsPanel(this);
        case wxsDialogId:       return new wxsDialog(this);
        case wxsFrameId:        return new wxsFrame(this);
        case wxsPanelrId:       return new wxsPanelr(this);
    }
    
    return NULL;
}

/** Killing widget */
void wxsStdManagerT::KillWidget(wxsWidget* Widget)
{
    if ( Widget ) delete Widget;
}

wxsStdManagerT wxsStdManager;
