/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef NATIVEPARSERBASE_H
#define NATIVEPARSERBASE_H

#include <wx/string.h>

#include <map>
#include <queue>

#include "parser/parserthread.h" // g_UnnamedSymbol
#include "parser/token.h"
#include "parser/tokentree.h"

/** debug only variable, used to print the AI match related log message*/
extern bool s_DebugSmartSense;

class NativeParserBase
{
public:
    /** divide a statement to several pieces(parser component), each component has a type member*/
    enum ParserTokenType
    {
        pttUndefined = 0,
        pttSearchText,
        pttClass,
        pttNamespace,
        pttFunction,
    };

    /** the delimiter separating two Parser Component, See ParserComponent struct for more details */
    enum OperatorType
    {
        otOperatorUndefined = 0,
        otOperatorSquare,
        otOperatorParentheses,
        otOperatorPointer,
        otOperatorStar
    };

    /** @brief a long statement can be divided to a ParserComponent chain.
     *
     * e.g. for a statement like below:
     * Ogre::Root::getSingleton().|
     *
     *  a chains of four ParserComponents will be generated and list below:
     *  Ogre             [pttNamespace]
     *  Root             [pttClass]
     *  getSingleton     [pttFunction]
     *  (empty space)    [pttSearchText]
     */
    struct ParserComponent
    {
        wxString        component;  /// name
        ParserTokenType tokenType;          /// type
        OperatorType    tokenOperatorType;/// operator type

        ParserComponent() { Clear(); }
        void Clear()
        {
            component         = wxEmptyString;
            tokenType         = pttUndefined;
            tokenOperatorType = otOperatorUndefined;
        }
    };

    /** Constructor */
    NativeParserBase();

    /** Destructor */
    virtual ~NativeParserBase();

protected:

    /** Init cc search member variables */
    void Reset();

    /**@brief Artificial Intelligence Matching
    *
    * All functions that call this recursive function, should already entered a critical section.
    *
    * match (consume) the ParserComponent queue from left to right,
    * the output result becomes the search scope of the next match.
    * finally, give the result matching the last ParserComponent.
    * @param components input ParserComponent queue
    * @param parentTokenIdx, initial search scope of the left most component
    * @param fullMatch the result should be a full text match or prefix match
    * @return matching token number
    */
    size_t FindAIMatches(TokenTree*                  tree,
                         std::queue<ParserComponent> components,
                         TokenIdxSet&                result,
                         int                         parentTokenIdx = -1,
                         bool                        isPrefix = false,
                         bool                        caseSensitive = false,
                         bool                        use_inheritance = true,
                         short int                   kindMask = 0xFFFF,
                         TokenIdxSet*                search_scope = 0);

    void FindCurrentFunctionScope(TokenTree*         tree,
                                  const TokenIdxSet& procResult,
                                  TokenIdxSet&       scopeResult);

    void CleanupSearchScope(TokenTree*  tree,
                            TokenIdxSet* searchScope);

    /** Returns the start and end of the call-tip highlight region. */
    void GetCallTipHighlight(const wxString& calltip,
                             int*            start,
                             int*            end,
                             int             typedCommas);

    /** for GetCallTipHighlight()
        Finds the position of the opening parenthesis marking the beginning of the params. */
    int FindFunctionOpenParenthesis(const wxString& calltip);

    /** Decides if the token belongs to its parent or one of its ancestors */
    bool BelongsToParentOrItsAncestors(TokenTree*   tree,
                                       const Token* token,
                                       int          parentIdx,
                                       bool         use_inheritance = true);

    /** helper function to split the statement*/
    wxString GetCCToken(wxString&        line,
                        ParserTokenType& tokenType,
                        OperatorType&    tokenOperatorType);

    /** helper function to split the statement*/
    unsigned int FindCCTokenStart(const wxString& line);

    /** helper function to read the next CCToken */
    wxString GetNextCCToken(const wxString& line,
                            unsigned int&   startAt,
                            OperatorType&   tokenOperatorType);

    /** Remove the last function's childrens */
    void RemoveLastFunctionChildren(TokenTree* tree, int& lastFuncTokenIdx);

    /** @brief break a statement to several ParserComponents, and store them in a queue.
     * @param actual a statement string to be divided.
     * @param components output variable containing the queue.
     * @return number of ParserComponent
     */
    size_t BreakUpComponents(const wxString&              actual,
                             std::queue<ParserComponent>& components);

    /** A statement(expression) is expressed by a ParserComponent queue
     *  We do a match from the left of the queue one by one.
     *
     * @param components expression structure expressed in std::queue<ParserComponent>
     * @param searchScope search scope defined by TokenIdxSet
     * @param caseSense case sensitive match
     * @param isPrefix match type( full match or prefix match)
     * @return result tokens count
     */
    size_t ResolveExpression(TokenTree*                  tree,
                             std::queue<ParserComponent> components,
                             const TokenIdxSet&          searchScope,
                             TokenIdxSet&                result,
                             bool                        caseSense = true,
                             bool                        isPrefix = false);

    /** used to solve the overloaded operator functions return type
     * @param tokenOperatorType overloaded operator type, could be [], (), ->
     * @param tokens input tokens set
     * @param tree Token tree pointer
     * @param searchScope search scope
     * @param result output result
     */
    void ResolveOperator(TokenTree*          tree,
                         const OperatorType& tokenOperatorType,
                         const TokenIdxSet&  tokens,
                         const TokenIdxSet&  searchScope,
                         TokenIdxSet&        result);

    /** Get the Type information of the searchText string
     * @param searchText input search text
     * @param searchScope search scope defined in TokenIdxSet format
     * @param result result token specify the Type of searchText
     */
    size_t ResolveActualType(TokenTree*         tree,
                             wxString           searchText,
                             const TokenIdxSet& searchScope,
                             TokenIdxSet&       result);

    /** resolve template map [formal parameter] to [actual parameter]
     * @param searchStr input Search String
     * @param actualtypeScope Token type(actual parameter)
     * @param initialScope search scope
     */
    void ResolveTemplateMap(TokenTree*         tree,
                            const wxString&    searchStr,
                            const TokenIdxSet& actualTypeScope,
                            TokenIdxSet&       initialScope);

    /** add template parameter, get the actual parameter from the formal parameter list
     * @param id template token id
     * @param actualTypeScope search scope
     * @param initialScope resolved result
     * @param tree Token tree pointer.
     */
    void AddTemplateAlias(TokenTree*         tree,
                          const int&         id,
                          const TokenIdxSet& actualTypeScope,
                          TokenIdxSet&       initialScope);

    /** Generate the matching results under the Parent Token index set
     *
     * All functions that call this recursive function, should already entered a critical section.
     *
     * @param tree TokenTree pointer
     * @param target Scope (defined in TokenIdxSet)
     * @param result result token index set
     * @param isPrefix whether a full match is used or only do a prefix match
     * @param kindMask define the result tokens filter, such as only class type is OK
     * @return result token set number
     */
    size_t GenerateResultSet(TokenTree*      tree,
                             const wxString& target,
                             int             parentIdx,
                             TokenIdxSet&    result,
                             bool            caseSens = true,
                             bool            isPrefix = false,
                             short int       kindMask = 0xFFFF);

    /** This function is just like the one above, especially that no Token tree information is used
     * So, it use the current parser's Token tree.
     *
     * All functions that call this recursive function, should already entered a critical section.
     *
     */
    size_t GenerateResultSet(TokenTree*         tree,
                             const wxString&    target,
                             const TokenIdxSet& ptrParentID,
                             TokenIdxSet&       result,
                             bool               caseSens = true,
                             bool               isPrefix = false,
                             short int          kindMask = 0xFFFF);

    /** Collect search scopes, add the searchScopes's parent scope
     * @param searchScope input search scope
     * @param actualTypeScope returned search scope
     * @param tree TokenTree pointer
     */
    void CollectSearchScopes(const TokenIdxSet& searchScope,
                             TokenIdxSet&       actualTypeScope,
                             TokenTree*         tree);

    /** used to get the correct token index in current line, e.g. class A { void test() { | } };
     * @param tokens all current file's function and class
     * @param curLine the line of the current position
     */
    int GetTokenFromCurrentLine(TokenTree*         tree,
                                const TokenIdxSet& tokens,
                                size_t             curLine,
                                const wxString&    file);

    void ComputeCallTip(TokenTree*         tree,
                        const TokenIdxSet& tokens,
                        int                chars_per_line,
                        wxArrayString&     items);

    /** For ComputeCallTip()
     * No critical section needed in this recursive function!
     * All functions that call this recursive function, should already entered a critical section. */
    bool PrettyPrintToken(const TokenTree*   tree,
                          const Token*       token,
                          wxString&          result,
                          bool               isRoot = true);

    /** helper function to split the statement*/
    void BreakUpInLines(wxString&       str,
                        const wxString& original_str,
                        int             chars_per_line = -1);

    // convenient static funcs for fast access and improved readability

    // count commas in lineText (nesting parentheses)
    static int CountCommas(const wxString& lineText, int start)
    {
        int commas = 0;
        int nest = 0;
        while (true)
        {
            wxChar c = lineText.GetChar(start++);
            if (c == '\0')
                break;
            else if (c == '(')
                ++nest;
            else if (c == ')')
                --nest;
            else if (c == ',' && nest == 1)
                ++commas;
        }
        return commas;
    }

    static bool InsideToken(int startAt, const wxString& line)
    {
        return (   (startAt >= 0)
                && ((size_t)startAt < line.Len())
                && (   (wxIsalnum(line.GetChar(startAt)))
                    || (line.GetChar(startAt) == '_') ) );
    }
    static int BeginOfToken(int startAt, const wxString& line)
    {
        while (   (startAt >= 0)
               && ((size_t)startAt < line.Len())
               && (   (wxIsalnum(line.GetChar(startAt)))
                   || (line.GetChar(startAt) == '_') ) )
            --startAt;
        return startAt;
    }
    static int BeforeToken(int startAt, const wxString& line)
    {
        if (   (startAt > 0)
            && ((size_t)startAt < line.Len() + 1)
            && (   (wxIsalnum(line.GetChar(startAt - 1)))
                || (line.GetChar(startAt - 1) == '_') ) )
            --startAt;
        return startAt;
    }
    static bool IsOperatorEnd(int startAt, const wxString& line)
    {
        return (   (startAt > 0)
                && ((size_t)startAt < line.Len())
                && (   (   (line.GetChar(startAt) == '>')
                        && (line.GetChar(startAt - 1) == '-') )
                    || (   (line.GetChar(startAt) == ':')
                        && (line.GetChar(startAt - 1) == ':') ) ) );
    }
    static bool IsOperatorPointer(int startAt, const wxString& line)
    {
        return (   (startAt > 0)
            && ((size_t)startAt < line.Len())
            && (   (   (line.GetChar(startAt) == '>')
                    && (line.GetChar(startAt - 1) == '-') )));
    }
    static bool IsOperatorBegin(int startAt, const wxString& line)
    {
        return (   (startAt >= 0)
                && ((size_t)startAt < line.Len())
                && (   (   (line.GetChar(startAt ) == '-')
                        && (line.GetChar(startAt + 1) == '>') )
                    || (   (line.GetChar(startAt) == ':')
                        && (line.GetChar(startAt + 1) == ':') ) ) );
    }
    static bool IsOperatorDot(int startAt, const wxString& line)
    {
        return (   (startAt >= 0)
                && ((size_t)startAt < line.Len())
                && (line.GetChar(startAt) == '.') );
    }
    static int BeforeWhitespace(int startAt, const wxString& line)
    {
        while (   (startAt >= 0)
               && ((size_t)startAt < line.Len())
               && (   (line.GetChar(startAt) == ' ')
                   || (line.GetChar(startAt) == '\t') ) )
            --startAt;
        return startAt;
    }
    static int AfterWhitespace(int startAt, const wxString& line)
    {
        if (startAt < 0)
            startAt = 0;
        while (   ((size_t)startAt < line.Len())
               && (   (line.GetChar(startAt) == ' ')
                   || (line.GetChar(startAt) == '\t') ) )
            ++startAt;
        return startAt;
    }
    static bool IsOpeningBracket(int startAt, const wxString& line)
    {
        return (   ((size_t)startAt < line.Len())
                && (   (line.GetChar(startAt) == '(')
                    || (line.GetChar(startAt) == '[') ) );
    }
    static bool IsClosingBracket(int startAt, const wxString& line)
    {
        return (   (startAt >= 0)
                && (   (line.GetChar(startAt) == ')')
                    || (line.GetChar(startAt) == ']') ) );
    }

protected:

private:
    // Helper utilities called only by GenerateResultSet!
    // No critical section needed! All functions that call these functions,
    // should already entered a critical section.

    // for GenerateResultSet()
    bool AddChildrenOfUnnamed(TokenTree* tree, const Token* parent, TokenIdxSet& result)
    {
        if (((parent->m_TokenKind & (tkClass | tkEnum)) != 0) && parent->m_Name.StartsWith(g_UnnamedSymbol))
        {
            // add all its children
            for (TokenIdxSet::const_iterator it = parent->m_Children.begin();
                                             it != parent->m_Children.end(); ++it)
            {
                Token* tokenChild = tree->at(*it);
                if (tokenChild &&
                    (parent->m_TokenKind == tkClass || tokenChild->m_Scope != tsPrivate))
                    result.insert(*it);
            }

            return true;
        }
        return false;
    }

    bool AddChildrenOfEnum(TokenTree* tree, const Token* parent, TokenIdxSet& result)
    {
        if (parent->m_TokenKind == tkEnum)
        {
            // add all its children
            for (TokenIdxSet::const_iterator it = parent->m_Children.begin();
                                             it != parent->m_Children.end(); ++it)
            {
                Token* tokenChild = tree->at(*it);
                if (tokenChild && tokenChild->m_Scope != tsPrivate)
                    result.insert(*it);
            }

            return true;
        }
        return false;
    }

    // for GenerateResultSet()
    bool MatchText(const wxString& text, const wxString& target, bool caseSens, bool isPrefix)
    {
        if (isPrefix && target.IsEmpty())
            return true;
        if (!isPrefix)
            return text.CompareTo(target, caseSens ? wxString::exact : wxString::ignoreCase) == 0;
        // isPrefix == true
        if (caseSens)
            return text.StartsWith(target);
        return text.Upper().StartsWith(target.Upper());
    }

    // for GenerateResultSet()
    bool MatchType(TokenKind kind, short int kindMask)
    {
        return kind & kindMask;
    }

private:
    ParserComponent              m_LastComponent;
    std::map<wxString, wxString> m_TemplateMap;
};

#endif // NATIVEPARSERBASE_H
