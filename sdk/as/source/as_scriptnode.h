/*
   AngelCode Scripting Library
   Copyright (c) 2003-2005 Andreas J�nsson

   This software is provided 'as-is', without any express or implied 
   warranty. In no event will the authors be held liable for any 
   damages arising from the use of this software.

   Permission is granted to anyone to use this software for any 
   purpose, including commercial applications, and to alter it and 
   redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you 
      must not claim that you wrote the original software. If you use
	  this software in a product, an acknowledgment in the product 
	  documentation would be appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and 
      must not be misrepresented as being the original software.

   3. This notice may not be removed or altered from any source 
      distribution.

   The original version of this library can be located at:
   http://www.angelcode.com/angelscript/

   Andreas J�nsson
   andreas@angelcode.com
*/


//
// as_scriptnode.h
//
// A node in the script tree built by the parser for compilation
//


#ifndef AS_SCRIPTNODE_H
#define AS_SCRIPTNODE_H

#include "as_tokendef.h"

enum eScriptNode
{
	snUndefined,
	snScript,
	snFunction,
	snConstant,
	snDataType,
	snIdentifier,
	snParameterList,
	snStatementBlock,
	snDeclaration,
	snExpressionStatement,
	snIf,
	snFor,
	snWhile,
	snReturn,
	snExpression,
	snExprTerm,
	snFunctionCall,
	snArgList,
	snExprPreOp,
	snExprPostOp,
	snExprOperator,
	snExprValue,
	snBreak,
	snContinue,
	snDoWhile,
	snAssignment,
	snCondition,
	snGlobalVar,
	snSwitch,
	snCase,
	snImport,
	snStruct,
	snInitList
};

struct sToken
{
	eTokenType type;
	int pos;
	int length;
};

class asCScriptNode
{
public:
	asCScriptNode(eScriptNode nodeType);
	~asCScriptNode();

	void SetToken(sToken *token);
	void AddChildLast(asCScriptNode *node);
	void DisconnectParent();

	void UpdateSourcePos(int pos, int length);

	eScriptNode nodeType;
	eTokenType tokenType;
	int tokenPos;
	int tokenLength;

	asCScriptNode *parent;
	asCScriptNode *next;
	asCScriptNode *prev;
	asCScriptNode *firstChild;
	asCScriptNode *lastChild;
};

#endif
