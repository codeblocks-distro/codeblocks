/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef CBAUIBOOK_H_INCLUDED
#define CBAUIBOOK_H_INCLUDED

#include <wx/aui/auibook.h>
#include <wx/dynarray.h>

class wxTipWindow;
class cbAuiNotebook;

WX_DEFINE_ARRAY_PTR(wxAuiTabCtrl*,cbAuiTabCtrlArray);
WX_DEFINE_ARRAY_PTR(cbAuiNotebook*,cbAuiNotebookArray);

/** \brief A notebook class
  * This class is derived from wxAuiNotebook, to enhance its abilities.
  * It adds the ability to store (and restore) the visible tab-order, because
  * wxAuiNotebook-tabs can be reordered with drag and drop.
  * Another added feature is the possibility to add tooltips to the tabs belonging
  * to added panes.
  */
class cbAuiNotebook : public wxAuiNotebook
{
    public:
        /** \brief cbAuiNotebook constructor
         *
         * \param pParent the parent window, usually the app-window
         * \param id the notebook id
         * \param pos the position
         * \param size the size
         * \param style the notebook style
         */
        cbAuiNotebook(wxWindow* pParent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxAUI_NB_DEFAULT_STYLE);
        /** cbAuiNotebook destructor  */
        virtual ~cbAuiNotebook();

        /** \brief Advances the selection
         *
         * In contrast to the base-classes function, it uses the visible tab-order, not the order
         * of creation and jumps to the first tab, if the last is reached (and vice versa)
         * \param forward if false direction is backwards
         */
        void AdvanceSelection(bool forward = true);
        /** \brief Save layout of the notebook
         * \return wxString the serialized layout
         * @remarks Not used at the moment, because it's not (yet) possible to restore the layout,
         * due to limitations of the base class.
         */
        wxString SavePerspective();
        /** \brief Loads serialized notebook layout
         * \param layout the serialized layout
         * \return bool true if successfull
         * @remarks Not implemented. Don't use it.
         *
         */
        bool LoadPerspective(const wxString& /*layout*/) {return false;};
        /** \brief Get the tab position
         *
         * Returns the position of the tab as it is visible.
         * Starts with 0
         * \param index the index of the tab in order of creation
         * \return int the visible position
         */
        int GetTabPositionFromIndex(int index);
        /** \brief Set a tab tooltip
         *
         * Sets the tooltip for the tab belonging to win.
         * Starts the dwell timer and the stopwatch if it is not already done.
         * \param win the pane that belongs to the tab
         * \param msg the tooltip
         * @remarks Uses the name of the wxWindow to store the message
         */
        void SetTabToolTip(wxWindow* win, wxString msg);
        /** \brief Allow tooltips
         *
         * Allows or forbids tooltips.
         * Cancels already shown tooltips, if allow is false
         * \param allow if false toltips are not allowed
         */
        void AllowToolTips(bool allow = true);
        /** \brief Minmize free horizontal page
         *
         * Moves the active tab of all tabCtrl's to the rightmost place,
         * to show as many tabs as possible.
         */
        void MinimizeFreeSpace();
        /** \brief Delete Page
         *
         * Calls the base-class function and after that
         * MinmizeFreeSpace(), needed to hook into the close-events.
         * The system generated close event has to be veto'd, and Close()
         * has to be called manually, so we can handle it ourselves.
         * \param The index of the tab to be closed
         * \return true if successfull
         */
        bool DeletePage(size_t page);
        /** \brief Remove Page
         *
         * Calls the base-class function and after that
         * MinmizeFreeSpace(), needed to hook into the close-events.
         * The system generated close event has to be veto'd, and Close()
         * has to be called manually, so we can handle it ourselves.
         * \param The index of the tab to be closed
         * \return true if successfull
         */
        bool RemovePage(size_t page);
        /** \brief Move page
         *
         * Moves the tab containing page to new_idx
         * \param page The page to move (e.g. cbEditor*)
         * \param new_idx The index the page should be moved to
         * \return true if successfull
         */
        bool MovePage(wxWindow* page, size_t new_idx);
        /** \brief Set zoomfactor for builtin editors
         *
         * Sets the zoomfactor for all visible builtin
         * editors.
         * \param zoom zoomfactor to use
         */
        void SetZoom(int zoom);
        /** \brief Set Focus on the tabCtrl belonging to the active tab
         */
        void FocusActiveTabCtrl();
    protected:
        /** \brief Minmize free horizontal page of tabCtrl
         *
         * Moves the active tab of tabCtrl to the rightmost place,
         * to show as many tabs as possible.
         * \param tabCtrl The tabCtrl to act on
         */
        void MinimizeFreeSpace(wxAuiTabCtrl* tabCtrl);
        /** \brief Handle the navigation key event
         *
         * Tries to handle the navigation key-event and use "our" AdvanceSelection().
         * \param event
         * @remarks Works not reliable, due to OS/wxWidgets-limitations
         */
#if wxCHECK_VERSION(2, 9, 0)
        void OnNavigationKeyNotebook(wxNavigationKeyEvent& event);
#else
        void OnNavigationKey(wxNavigationKeyEvent& event);
#endif
        /** \brief OnIdle
         *
         * \param event unused
         */
        void OnIdle(wxIdleEvent& /*event*/);
        /** \brief Check whether the mouse is over a tab
         *
         * \param event unused
         */
        void OnDwellTimerTrigger(wxTimerEvent& /*event*/);
        /** \brief Catch doubleclick-events from wxTabCtrl
         *
         * Sends cbEVT_CBAUIBOOK_LEFT_DCLICK, if doubleclick was on a tab,
         * event-Id is the notebook-Id, event-object is the pointer to the window the
         * tab belongs to.
         * \param event holds the wxTabCtrl, that sends the event
         */
        void OnTabCtrlDblClick(wxMouseEvent& event);
        /** \brief Catch mousewheel-events from wxTabCtrl
         *
         * Sends cbEVT_CBAUIBOOK_MOUSEWHEEL, if doubleclick was on a tab,
         * event-Id is the notebook-Id, event-object is the pointer to the window the
         * tab belongs to.
         * \param event holds the wxTabCtrl, that sends the event
         */
        void OnTabCtrlMouseWheel(wxMouseEvent& event);
        /** \brief Catch mousewheel-events from tooltipwindow
         *
         * Closes Tooltip.
         * \param event the tipwindow, that sends the event
         */
        void OnToolTipMouseWheel(wxMouseEvent& event);
        /** \brief Catch resize-events and call MinimizeFreeSpace()
         *
         * \param event unused
         */
        void OnResize(wxSizeEvent& event);
        /** \brief Catch mouseenter-events from wxTabCtrl
         *
         * Set focus on wxTabCtrl
         * \param event holds the wxTabCtrl, that sends the event
         */
        void OnEnterTabCtrl(wxMouseEvent& event);
        /** \brief Catch mouseleave-events from wxTabCtrl
         *
         * \param event holds the wxTabCtrl, that sends the event
         */
        void OnLeaveTabCtrl(wxMouseEvent& event);
#ifdef __WXMSW__
        // hack needed on wxMSW, because only focused windows get mousewheel-events
        /** \brief Checks the old focus
         *
         * Checks whether the old focused window or one of it's
         * parents is the same as page.
         * If they are equal, we have to reset the stored pointer,
         * because we get a crash otherwise.
         * \param page The page to check against
         * \return bool
         */
        bool IsFocusStored(wxWindow* page);
        /** \brief Save old focus
         *
         * Save old focus and tab-selection,
         * \param event holds the wxTabCtrl, that sends the event
         */
        void StoreFocus();
        /** \brief Restore old focus
         *
         * Restore old focus or set the focus on the activated tab
         * \param event holds the wxTabCtrl, that sends the event
         */
        void RestoreFocus();
#endif // #ifdef __WXMSW__
        /** \brief Reset tabctrl events
         */
        void ResetTabCtrlEvents();
        /** \brief Updates the array, that holds the wxTabCtrls
         */
        void UpdateTabControlsArray();
        /** \brief Shows tooltip for win
         *
         * \param win
         */
        void ShowToolTip(wxWindow* win);
        /** \brief Cancels tooltip
         */
        void CancelToolTip();
        /** \brief Check for pressed modifier-keys
         *
         * Check whether all modifier keys in keyModifier are pressed
         * or not
         * \param keyModifier wxSTring containing the modifier(s) to check for
         * \return true If all modifier-keys are pressed
         */
        bool CheckKeyModifier();
        /** \brief Holds the wxTabCtrls used by the notebook
         * @remarks Should be updated with UpdateTabControlsArray(),
         * before it's used
         */
        cbAuiTabCtrlArray m_TabCtrls;
        /** \brief Stopwatch used to determine how long the mouse has not
         * been moved over a tab.
         */
        wxStopWatch m_StopWatch;
        /** \brief Timer used to check the mouse-position
         */
        wxTimer* m_pDwellTimer;
        /** \brief The actual shown tooltip or nullptr
         */
        wxTipWindow* m_pToolTip;
        /** \brief Position of the mouse, at last dwell timmer event.
         *
         * Used to determine whether the mouse was moved or not.
         */
        wxPoint m_LastMousePosition;
        /** \brief Position of tooltip
         *
         * Used to determine, whether a tooltiop is already shown at
         * the actual mouse-position
         */
        wxPoint m_LastShownAt;
        /** \brief Last time the dwell timer triggered an event
         *
         * Used to determine how long the mouse has not been moved over a tab .
         */
        long m_LastTime;
#ifdef __WXMSW__
        // needed for wxMSW-hack, see above
        /** \brief Last selected tab
         *
         * Used to determine whether the tab-selection has changed btween mouseenter
         * and mouseleave-event.
         */
        int m_LastSelected;
        /** \brief Id of last focused window
         *
         * Used to restore the focus after a mouseleave-event on wxTabCtrl.
         */
        long m_LastId;
#endif // #ifdef __WXMSW__
        /** \brief If false, tooltips are temporary forbidden
         *
         * Needed to not interfere with context-menus etc.
         */
        bool m_AllowToolTips;
        /** \brief If true, mouse pointer is over a tabCtrl
         */
        bool m_OverTabCtrl;
        /** \brief If true, zoom for all editors
         * is set in next OnIdle-call
         *
         */
        bool m_SetZoomOnIdle;
        /** \brief Holds the id of the dwell timer
         */
        long m_IdNoteBookTimer;
        /** \brief Holds the size of a tabCtrl after a resize event
         *
         * Needed to skip a resize event, if size did not change
         * it gets triggered on any tab-click
         */
        wxSize m_TabCtrlSize;

//static stuff (common to all cbAuiNotebooks)
    public:
        /** \brief Enable or disable tabtooltips globally
         *
         * \param use If true tooltips are allowed
         */
        static void UseToolTips(bool use = true);
        /** \brief Set the time before a tab-tooltip kicks in
         *
         * \param time The dwell time
         */
        static void SetDwellTime(long time = 1000);
        /** \brief Enable or disable tab-scrolling with mousewheel
         *
         * \param allow If true scrolling is allowed
         */
        static void AllowScrolling(bool allow = true);
        /** \brief Sets the modifier keys for scrolling
         */
        static void SetModKeys(wxString keys = _T("Strg"));
        /** \brief Use modkey to advance through tabs with mousewheel
         */
        static void UseModToAdvance(bool use = false);
        /** \brief Change direction of tab-advancing with mousewheel
         *
         * \param invert If true advance direction is inverted
         */
        static void InvertAdvanceDirection(bool invert = false);
        /** \brief Change direction of tab-moving with mousewheel
         *
         * \param invert If true move direction is inverted
         */
        static void InvertMoveDirection(bool invert = false);
    protected:
        /** \brief Enable or disable tab tooltips
         */
        static bool s_UseTabTooltips;
        /** \brief Tab tooltip dwell time
         */
        static long s_DwellTime;
        /** \brief Enable or disable scrolling tabs with mousewheel
         */
        static bool s_AllowMousewheel;
        /** \brief Holds an array of all existing cbAuiNotebooks
         */
        static cbAuiNotebookArray s_cbAuiNotebookArray;
        /** \brief Holds the modifier keys for scrolling
         */
        static wxString s_modKeys;
        /** \brief Use modkey to advance through tabs with mousewheel
         */
        static bool s_modToAdvance;
        /** \brief Mousewheel move direction: negative => invert
         */
        static int s_moveDirection;
        /** \brief Mouseweheel advance direction: negative => invert
         */
        static int s_advanceDirection;

        DECLARE_EVENT_TABLE()
};

#endif // CBAUIBOOK_H_INCLUDED
