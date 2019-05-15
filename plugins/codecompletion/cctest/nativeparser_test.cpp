#include <sdk.h>

#ifndef CB_PRECOMP
#endif

#include "nativeparser_test.h"

#include "parser/cclogger.h"

#include <wx/textfile.h>
#include <wx/tokenzr.h>

#define CC_NATIVEPARSERTEST_DEBUG_OUTPUT 0

#if defined(CC_GLOBAL_DEBUG_OUTPUT)
    #if CC_GLOBAL_DEBUG_OUTPUT == 1
        #undef CC_NATIVEPARSERTEST_DEBUG_OUTPUT
        #define CC_NATIVEPARSERTEST_DEBUG_OUTPUT 1
    #elif CC_GLOBAL_DEBUG_OUTPUT == 2
        #undef CC_NATIVEPARSERTEST_DEBUG_OUTPUT
        #define CC_NATIVEPARSERTEST_DEBUG_OUTPUT 2
    #endif
#endif

#ifdef CC_PARSER_TEST
    #define ADDTOKEN(format, args...) \
            wxLogMessage(F(format, ##args))
    #define TRACE(format, args...) \
            wxLogMessage(F(format, ##args))
    #define TRACE2(format, args...) \
            wxLogMessage(F(format, ##args))
#else
    #if CC_NATIVEPARSERTEST_DEBUG_OUTPUT == 1
        #define ADDTOKEN(format, args...) \
                CCLogger::Get()->AddToken(F(format, ##args))
        #define TRACE(format, args...) \
            CCLogger::Get()->DebugLog(F(format, ##args))
        #define TRACE2(format, args...)
    #elif CC_NATIVEPARSERTEST_DEBUG_OUTPUT == 2
        #define ADDTOKEN(format, args...) \
                CCLogger::Get()->AddToken(F(format, ##args))
        #define TRACE(format, args...)                                              \
            do                                                                      \
            {                                                                       \
                if (g_EnableDebugTrace)                                             \
                    CCLogger::Get()->DebugLog(F(format, ##args));                   \
            }                                                                       \
            while (false)
        #define TRACE2(format, args...) \
            CCLogger::Get()->DebugLog(F(format, ##args))
    #else
        #define ADDTOKEN(format, args...)
        #define TRACE(format, args...)
        #define TRACE2(format, args...)
    #endif
#endif

bool s_DebugSmartSense = false; // if true, then cclogger will log many debug messages

namespace CCTestAppGlobal
{
    extern wxArrayString s_includeDirs;
    extern wxArrayString s_fileQueue;
    extern wxArrayString s_filesParsed;
}// CCTestAppGlobal

NativeParserTest::NativeParserTest()
{
    m_Parser.Options().wantPreprocessor = true; // Default
    m_Parser.Options().storeDocumentation = true; // Default
}

NativeParserTest::~NativeParserTest()
{
}

bool NativeParserTest::TestExpression(wxString&          expression,
                                      const TokenIdxSet& searchScope,
                                      TokenIdxSet&       result)
{
    // find all other matches
    std::queue<ParserComponent> components;
    BreakUpComponents(expression, components);

    ResolveExpression(m_Parser.GetTokenTree(), components, searchScope, result, true, true /*isPrefix*/);

    if (s_DebugSmartSense)
        CCLogger::Get()->DebugLog(F(_T("NativeParserTest::TestExpression, returned %lu results"), static_cast<unsigned long>(result.size())));

    return true;
}

bool NativeParserTest::Parse(wxString& file, bool isLocalFile)
{
    if (isLocalFile)
        return m_Parser.Reparse(file, true);

    return m_Parser.ParseBuffer(file, true);
}

void NativeParserTest::PrintList()
{
    const TokenList* tokens = m_Parser.GetTokenTree()->GetTokens();
    for (TokenList::const_iterator it=tokens->begin(); it!=tokens->end(); ++it)
    {
        Token* token = (*it);
        if (token)
        {
            wxString log;
            log << token->GetTokenKindString() << _T(" ")
                << token->DisplayName()        << _T("\t[")
                << token->m_Line               << _T(",")
                << token->m_ImplLine           << _T("]");
            CCLogger::Get()->Log(log);
        }
    }
}

wxString NativeParserTest::SerializeTree()
{
    return m_Parser.GetTokenTree()->m_Tree.Serialize();
}

void NativeParserTest::PrintTokenTree(Token* token)
{
    if (!token)
        return;

    wxString log;
    if (!token->m_Children.empty()) log << _T("+");
    if (token->m_TokenKind == tkFunction)
        log << token->m_Name << token->m_Args << _T("\t");
    else
        log << token->DisplayName() << _T("\t");
    log << _T("[") << token->m_Line << _T(",") << token->m_ImplLine << _T("]");
    CCLogger::Get()->Log(log);

    TokenIdxSet& ids = token->m_Children;
    for (TokenIdxSet::iterator it=ids.begin(); it!=ids.end(); ++it)
    {
        Token* token_ex = m_Parser.GetTokenTree()->at(*it);
        PrintTokenTree(token_ex); // recursion
    }
}

void NativeParserTest::PrintTree()
{
    const TokenList *tokens = m_Parser.GetTokenTree()->GetTokens();
    for (TokenList::const_iterator it=tokens->begin(); it!=tokens->end(); ++it)
    {
        Token* token = (*it);
        if (token)
        {
          Token* parent = m_Parser.GetTokenTree()->at(token->m_ParentIndex);
          if (!parent)
              PrintTokenTree(token);
        }
    }
}

void NativeParserTest::Clear()
{
    m_Parser.GetTokenTree()->clear();
}

void NativeParserTest::Init()
{
    // initialize the include files
    // second, try taking include directories into account
    for (size_t i=0; i<CCTestAppGlobal::s_includeDirs.GetCount(); i++)
    {
        wxString include_dir = CCTestAppGlobal::s_includeDirs.Item(i);
        m_Parser.AddIncludeDir(include_dir);
    }
    // add the "testing" folder as include search path
    m_Parser.AddIncludeDir(wxGetCwd()+wxT("/testing"));
}

bool NativeParserTest::ParseAndCodeCompletion(wxString filename, bool isLocalFile)
{
    Clear(); //clear the tree

    bool parseResult = false;
    parseResult = Parse(filename, isLocalFile);
    if (!parseResult)
        return false;

    int passCount = 0;
    int failCount = 0;

    wxString testResult;
    wxString message;

    if (isLocalFile)
    {
        // only print the base name, so we can save the log in our source control system
        wxString baseName;
        wxString ext;
        wxFileName::SplitPath(filename, nullptr, &baseName, &ext);
        wxString printName = baseName + _T(".") + ext;
        message = wxString::Format(_T("********************************************************\n  Testing in file: %s\n********************************************************"), printName.wx_str());
    }
    else
        message = wxString::Format(_T("********************************************************\n  Testing file in edit control\n********************************************************"));

    wxLogMessage(message);
    testResult << message << wxT("\n");

    // reading the test cases, first we read all the lines of the file
    // handling local files and wxScintilla control differently
    std::vector<wxString> allLines;
    if (isLocalFile)
    {
        // read the test cases of CodeCompletion test
        wxTextFile source;
        source.Open(filename);
        wxString str;
        for ( str = source.GetFirstLine();
              source.GetCurrentLine() < source.GetLineCount();
              str = source.GetNextLine() )
        {
            allLines.push_back(str);
        }
    }
    else
    {
        wxStringTokenizer tokenizer(filename, wxT("\n"), wxTOKEN_RET_EMPTY);
        while ( tokenizer.HasMoreTokens() )
        {
            wxString token = tokenizer.GetNextToken();
            allLines.push_back(token);
        }
    }

    // the test cases are list as the last line of the file, so we loop backwards, and stop if an
    // empty line is found
    for (ssize_t l = allLines.size() - 1; l >= 0; l--)
    {
        wxString str = allLines[l];
        // a test case should be put in a line, and start with the double slash
        if (str.StartsWith(_T("//")))
        {
            // do tests here, example of line is below
            // tc.St    //StaticVoid
            // remove the beginning "//"
            str.Remove(0, 2);

            int pos;
            wxString expression;
            wxString match;
            wxString match_doc;

            // find the optional "///<" for Doxygen comment tests
            pos = str.Find(_T("///<"));
            if (pos != wxNOT_FOUND)
            {
                match_doc = str.Mid(pos + 4);
                str = str.Mid(0, pos);
            }

            // find the second "//", the string after the second double slash are the
            // the result should be listed
            pos = str.Find(_T("//"));
            if (pos != wxNOT_FOUND)
            {
                expression = str.Mid(0, pos);
                match = str.Mid(pos + 2);// the remaining string
            }
            else
            {
                expression = str;
                if (!match_doc.IsEmpty())
                    match = _T("* @doxygen");
            }

            expression.Trim(true).Trim(false);
            match.Trim(true).Trim(false);
            match_doc.Trim(true).Trim(false);

            wxArrayString suggestList;
            // the match can have many items, like: AAA,BBBB
            wxStringTokenizer tkz(match, wxT(","));
            while ( tkz.HasMoreTokens() )
            {
                wxString token = tkz.GetNextToken().Trim(true).Trim(false);
                suggestList.Add(token);
            }

            TokenIdxSet searchScope;
            searchScope.insert(-1);
            TokenIdxSet result;
            TestExpression(expression,searchScope,result);

            // loop the suggestList to see it is in the result Tokens
            for (size_t s=0; s<suggestList.GetCount(); s++)
            {
                wxString element = suggestList[s];
                bool pass = false; // pass the test?
                for (TokenIdxSet::const_iterator it = result.begin();
                     it != result.end();
                     ++it)
                {
                    const Token* token = m_Parser.GetTokenTree()->at(*it);
                    if (!token || token->m_Name.IsEmpty())
                        continue;

                    if (element.IsSameAs(token->m_Name) || element[0] == '*')
                    {
                        // no doxygen documents, only matches the suggestion list
                        if (match_doc.IsEmpty())
                        {
                            message = wxString::Format(_T("+ PASS: %s  %s"), expression.wx_str(), element.wx_str());
                            testResult << message << wxT("\n");
                            wxLogMessage(message);
                            pass = true;
                            ++passCount;
                        }
                        else
                        {
                            // check whether doxygen documents are matched
                            if (token->m_Doc.Contains(match_doc)
                            || (match_doc[0] == '*' && match_doc.Len() == 1 && !token->m_Doc.IsEmpty())
                            || (match_doc[0] == '-' && match_doc.Len() == 1 && token->m_Doc.IsEmpty()))
                            {
                                message = wxString::Format(_T("+ PASS: %s  %s  \"%s\""), expression.wx_str(), token->m_Name.wx_str(), match_doc.wx_str());
                                testResult << message << wxT("\n");
                                wxLogMessage(message);
                                if (!pass)
                                {
                                    pass = true;
                                    ++passCount;
                                }
                            }
                            else
                            {
                                if (pass)
                                    --passCount;
                                pass = false;
                                element = wxString::Format(_T("%s  \"%s\""), token->m_Name.wx_str(), match_doc.wx_str());
                                break;
                            }
                        }
                        if (element[0] != '*')
                            break;
                    }

                }
                if (pass == false)
                {
                    message = wxString::Format(_T("- FAIL: %s  %s"), expression.wx_str(), element.wx_str());
                    testResult << message << wxT("\n");
                    wxLogMessage(message);
                    failCount++;
                }
            }
            // wxLogMessage(_T("Result have %lu matches"), static_cast<unsigned long>(result.size()));
        }
        else
            break; // if the line is not started with //, then we just stop testing
    }

    // report the test result here again in the last stage, further more, we can show this in another text control

    wxLogMessage(wxT("--------------------------------------------------------\nTotal %d tests, %d PASS, %d FAIL\n--------------------------------------------------------"), passCount+failCount, passCount, failCount);

    return true;
}
