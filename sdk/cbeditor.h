#ifndef EDITOR_H
#define EDITOR_H

#define EDITOR_MODIFIED             "*"
#ifdef __WXMSW__
    #define DEFAULT_EDITOR_FONT 	"0;-13;0;0;0;400;0;0;0;0;3;2;1;49;Courier New"
#else
    #define DEFAULT_EDITOR_FONT 	"0;-monotype-courier new-medium-r-normal-*-*-110-*-*-m-*-iso8859-1"
#endif // __WXMSW__

#define BOOKMARK_MARKER					0
#define BOOKMARK_STYLE 					wxSTC_MARK_ARROW
#define BREAKPOINT_MARKER				1
#define BREAKPOINT_STYLE 				wxSTC_MARK_CIRCLE
#define BREAKPOINT_LINE					2
#define ACTIVE_LINE						3
#define ERROR_LINE						4

#include <wx/frame.h>
#include <wx/stc/stc.h>
#include <wx/hashmap.h>
#include <wx/datetime.h>

#include "settings.h"

// forward decls
class cbEditor;
class cbStyledTextCtrl;
class ProjectFile;
class EditorColorSet;
class wxNotebook;

WX_DECLARE_HASH_MAP(int, cbEditor*, wxIntegerHash, wxIntegerEqual, SwitchToMap);

class cbStyledTextCtrl : public wxStyledTextCtrl
{
	public:
		cbStyledTextCtrl(cbEditor* pParent, int id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0);
		virtual ~cbStyledTextCtrl();
	protected:
		void OnContextMenu(wxContextMenuEvent& event);
	private:
		cbEditor* m_pParent;
		DECLARE_EVENT_TABLE()
};

/**
 * This class represents one editor in Code::Studio. It holds all the necessary
 * information about an editor. When you want to access a Code::Studio editor,
 * this is the class you want to get at ;)\n
 * <em>Note: this class descends from wxPanel, so it provides all wxPanel methods
 * as well...</em>
 */
class DLLIMPORT cbEditor : public wxMDIChildFrame
{
    DECLARE_EVENT_TABLE()
    	friend class EditorManager;
	public:
		/** cbEditor constructor.
		  * @param parent the parent notebook - you should use EditorManager::Get()
		  * @param filename the filename to open. If filename is empty, it creates a
		  * new, empty, editor.
		  * @param theme the initial color set to use\n
		  * <em>Note: you should not create a cbEditor object directly. Instead
		  * use EditorManager's methods to do it...</em>
		  */
		cbEditor(wxMDIParentFrame* parent, const wxString& filename, EditorColorSet* theme = 0L);
		/** cbEditor destructor. */
		~cbEditor();

		// properties
		
		/** Returns a pointer to the underlying cbStyledTextCtrl object (which
		  * itself is the wxWindows implementation of Scintilla). If you want
		  * to mess with the actual contents of an editor, this is the object
		  * you want to get.
		  */
        cbStyledTextCtrl* GetControl(){ return m_pControl; }
		/** Returns true if editor is OK, i.e. constructor was called with a filename
		  * parameter and file was opened succesfully. If it returns false, you
		  * should delete the editor...
		  */
		bool IsOK(){ return m_IsOK; }
		/** Sets the editor title. For tabbed interface, it sets the corresponding
		  * tab text, while for MDI interface it sets the MDI window title...
		  */
		void SetEditorTitle(const wxString& title);
		/** <b>What is it doing here?</b> */
		void SetFilename(const wxString& x){ m_Filename = x; }
		/** Returns the editor's filename */
		const wxString& GetFilename(){ return m_Filename; }
		/** Returns the editor's short name. It is the name displayed on the 
		  * editor's tab...
		  */
		const wxString& GetShortName(){ return m_Shortname; }
		/** Returns true if editor is modified, false otherwise */
		bool GetModified(){ return m_Modified || m_pControl->GetModify(); }
		/** Set the editor's modification state to \c modified. */
		void SetModified(bool modified = true);
		/** Set the editor's page index in the parent wxNotebook */
		void SetPageIndex(int index){ m_Index = index; }
		/** Read the editor's page index in the parent wxNotebook */
		int GetPageIndex(){ return m_Index; }
		/** Set the ProjectFile pointer associated with this editor. All editors
		  * which belong to a project file, should have this set. All others should return NULL.
		  */
		void SetProjectFile(ProjectFile* project_file);
		/** Read the ProjectFile pointer associated with this editor. All editors
		  * which belong to a project file, have this set. All others return NULL.
		  */
		ProjectFile* GetProjectFile(){ return m_pProjectFile; }
		/** Updates the associated ProjectFile object with the editor's caret
		  * position, top visible line and its open state. Used in devProject
		  * layout information, so that each time the user opens a project
		  * file in the IDE, it opens exactly in the same state it was when last
		  * closed.
		  */
		void UpdateProjectFile();
		/** Save editor contents. Returns true on success, false otherwise. */
		bool Save();
		/** Save editor contents under a different filename. Returns true on success, false otherwise. */
		bool SaveAs();
		/** Unimplemented */
		bool RenameTo(const wxString& filename, bool deleteOldFromDisk = false);
		/** Fold all editor folds (hides blocks of code). */
		void FoldAll();
		/** Unfold all editor folds (shows blocks of code). */
		void UnfoldAll();
		/** Toggle all editor folds (inverts the show/hide state of blocks of code). */
		void ToggleAllFolds();
		/** Folds the block containing \c line. If \c line is -1, folds the block containing the caret. */
		void FoldBlockFromLine(int line = -1);
		/** Unfolds the block containing \c line. If \c line is -1, unfolds the block containing the caret. */
		void UnfoldBlockFromLine(int line = -1);
		/** Toggles folding of the block containing \c line. If \c line is -1, toggles folding of the block containing the caret. */
		void ToggleFoldBlockFromLine(int line = -1);
		/** Each line in the editor might have one or more markers associated with it.
		  * Think of it as flags for each line. A specific marker can be set for a line,
		  * or not set. Markers used in cbEditor are:
		  * \li breakpoints: set when a line has a breakpoint
		  * \li bookmarks: set when a line has a bookmark\n
		  * More markers may be used in the future...\n
		  * This method, sets a marker on a line.
		  * If \c line is -1, sets a marker on the line containing the caret.\n
		  * Predefined values for \c marker are BREAKPOINT_MARKER and BOOKMARK_MARKER...
		  */
		void MarkLine(int marker, int line = -1);
		/** Toggles \c marker on \c line.
		  * If \c line is -1, toggles \c marker on the line containing the caret.
		  * @see MarkLine() for information on markers.
		  */
		void MarkerToggle(int marker, int line = -1);
		/** Moves the caret to the next line containing marker of type \c marker.
		  * @see MarkLine() for information on markers.
		  */
		void MarkerNext(int marker);
		/** Moves the caret to the previous line containing marker of type \c marker.
		  * @see MarkLine() for information on markers.
		  */
		void MarkerPrevious(int marker);
		/** Sets the breakpoint markers of the editor, by asking the associated ProjectFile (which reads them from the project layout). */
		void SetBreakpoints();
		/** Set the color set to use. */
		void SetColorSet(EditorColorSet* theme);
		/** Highlights the brace pair (one of the braces must be under the cursor) */
		void HighlightBraces();
		/** Displays the editor's context menu (usually invoked by the user right-clicking in the editor) */
		void DisplayContextMenu(const wxPoint& position);
        /** Returns the specified line's (0-based) indentation (whitespace) in spaces. If line is -1, it uses the current line */
        int GetLineIndentInSpaces(int line = -1);
        /** Returns the specified line's (0-based) indentation (whitespace) string. If line is -1, it uses the current line */
        wxString GetLineIndentString(int line = -1);
        /** Returns the last modification time for the file. Used to detect modifications outside the editor. */
        wxDateTime GetLastModificationTime(){ return m_LastModified; }
        /** Reloads the file from disk. @return True on success, False on failure. */
        bool Reload();
    private:
        // functions
		void DoFoldAll(int fold); // 0=unfold, 1=fold, 2=toggle
		void DoFoldBlockFromLine(int line, int fold); // 0=unfold, 1=fold, 2=toggle
		bool DoFoldLine(int line, int fold); // 0=unfold, 1=fold, 2=toggle
        void CreateEditor();
        void SetEditorStyle();
        bool Open();
        wxString CreateUniqueFilename();
        void DoShowAutoComplete();
		bool LineHasMarker(int marker, int line = -1);
		wxColour GetOptionColour(const wxString& option, const wxColour _default);
		void NotifyPlugins(wxEventType type, int intArg = 0, const wxString& strArg = wxEmptyString, int xArg = 0, int yArg = 0);
        
        // events
        void OnMarginClick(wxStyledTextEvent& event);
        void OnEditorUpdateUI(wxStyledTextEvent& event);
        void OnEditorChange(wxStyledTextEvent& event);
        void OnEditorCharAdded(wxStyledTextEvent& event);
		void OnEditorDwellStart(wxStyledTextEvent& event);
		void OnEditorDwellEnd(wxStyledTextEvent& event);
		void OnUserListSelection(wxStyledTextEvent& event);
        void OnTimer(wxTimerEvent& event);
		void OnClose(wxCloseEvent& event);

		// one event handler for all popup menu entries
		void OnContextMenuEntry(wxCommandEvent& event);

        // variables
        bool m_IsOK;
        wxMDIParentFrame* m_pParent;
        cbStyledTextCtrl* m_pControl;
        int m_ID;
		wxString m_Filename;
		wxString m_Shortname;
		bool m_Modified;
		int m_Index;
        wxTimer m_timerWait;
		ProjectFile* m_pProjectFile;
		EditorColorSet* m_pTheme;
		short int m_ActiveCalltipsNest;
        SwitchToMap m_SwitchTo;
        wxDateTime m_LastModified; // to check if the file was modified outside the editor
};

#endif // EDITOR_H

