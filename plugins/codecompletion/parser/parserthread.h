#ifndef PARSERTHREAD_H
#define PARSERTHREAD_H

#include <wx/thread.h>
#include <wx/event.h>
#include <wx/string.h>
#include <wx/dynarray.h>
#include "tokenizer.h"
#include "token.h"

extern int THREAD_START;
extern int THREAD_END;
extern int NEW_TOKEN;
extern int FILE_NEEDS_PARSING;

static wxMutex s_mutexProtection;

struct ParserThreadOptions
{
	bool useBuffer;
	bool bufferSkipBlocks;
	bool wantPreprocessor;
};

class ParserThread : public wxThread
{
	public:
		ParserThread(wxEvtHandler* parent,
					const wxString& bufferOrFilename,
					bool isLocal,
					ParserThreadOptions& options,
					TokensArray* tokens);
		virtual ~ParserThread();
		bool Parse();
		bool ParseBufferForFunctions(const wxString& buffer);
		virtual void* Entry();
        virtual void SetTokens(TokensArray* tokens);
		const wxString& GetFilename(){ return m_Filename; }
	protected:
		wxChar SkipToOneOfChars(const wxString& chars, bool supportNesting = false);
		void SkipBlock();
		void HandleIncludes();
		void HandleDefines();
		void HandleNamespace();
		void HandleClass(bool isClass = true);
		void HandleFunction(const wxString& name, bool isOperator = false);
		void HandleEnum();
		Token* DoAddToken(TokenKind kind, const wxString& name, const wxString& args = wxEmptyString, bool isOperator = false);
		wxString GetActualTokenType();
	private:
		void Log(const wxString& log);
		Token* TokenExists(const wxString& name);
		Tokenizer m_Tokens;
		wxEvtHandler* m_pParent;
		TokensArray* m_pTokens;
		Token* m_pLastParent;
		TokenScope m_LastScope;
		wxString m_Filename;
		bool m_IsLocal;
		int m_StartBlockIndex;
		wxString m_Str;
		wxString m_LastToken;
		ParserThreadOptions m_Options;
};

#endif // PARSERTHREAD_H
