/* Scintilla source code edit control */
/** @file SciLexer.h
 ** Interface to the added lexer functions in the SciLexer version of the edit control.
 **/
/* Copyright 1998-2002 by Neil Hodgson <neilh@scintilla.org>
 * The License.txt file describes the conditions under which this software may be distributed. */

/* Most of this file is automatically generated from the Scintilla.iface interface definition
 * file which contains any comments about the definitions. HFacer.py does the generation. */

#ifndef SCILEXER_H
#define SCILEXER_H

/* SciLexer features - not in standard Scintilla */

/* ++Autogenerated -- start of section automatically generated from Scintilla.iface */
#define SCLEX_CONTAINER 0
#define SCLEX_NULL 1
#define SCLEX_PYTHON 2
#define SCLEX_CPP 3
#define SCLEX_HTML 4
#define SCLEX_XML 5
#define SCLEX_PERL 6
#define SCLEX_SQL 7
#define SCLEX_VB 8
#define SCLEX_PROPERTIES 9
#define SCLEX_ERRORLIST 10
#define SCLEX_MAKEFILE 11
#define SCLEX_BATCH 12
#define SCLEX_XCODE 13
#define SCLEX_LATEX 14
#define SCLEX_LUA 15
#define SCLEX_DIFF 16
#define SCLEX_CONF 17
#define SCLEX_PASCAL 18
#define SCLEX_AVE 19
#define SCLEX_ADA 20
#define SCLEX_LISP 21
#define SCLEX_RUBY 22
#define SCLEX_EIFFEL 23
#define SCLEX_EIFFELKW 24
#define SCLEX_TCL 25
#define SCLEX_NNCRONTAB 26
#define SCLEX_BULLANT 27
#define SCLEX_VBSCRIPT 28
#define SCLEX_BAAN 31
#define SCLEX_MATLAB 32
#define SCLEX_SCRIPTOL 33
#define SCLEX_ASM 34
#define SCLEX_CPPNOCASE 35
#define SCLEX_FORTRAN 36
#define SCLEX_F77 37
#define SCLEX_CSS 38
#define SCLEX_POV 39
#define SCLEX_LOUT 40
#define SCLEX_ESCRIPT 41
#define SCLEX_PS 42
#define SCLEX_NSIS 43
#define SCLEX_MMIXAL 44
#define SCLEX_CLW 45
#define SCLEX_CLWNOCASE 46
#define SCLEX_LOT 47
#define SCLEX_YAML 48
#define SCLEX_TEX 49
#define SCLEX_METAPOST 50
#define SCLEX_POWERBASIC 51
#define SCLEX_FORTH 52
#define SCLEX_ERLANG 53
#define SCLEX_OCTAVE 54
#define SCLEX_MSSQL 55
#define SCLEX_VERILOG 56
#define SCLEX_KIX 57
#define SCLEX_GUI4CLI 58
#define SCLEX_SPECMAN 59
#define SCLEX_AU3 60
#define SCLEX_APDL 61
#define SCLEX_BASH 62
#define SCLEX_ASN1 63
#define SCLEX_VHDL 64
#define SCLEX_CAML 65
#define SCLEX_BLITZBASIC 66
#define SCLEX_PUREBASIC 67
#define SCLEX_HASKELL 68
#define SCLEX_PHPSCRIPT 69
#define SCLEX_TADS3 70
#define SCLEX_REBOL 71
#define SCLEX_SMALLTALK 72
#define SCLEX_FLAGSHIP 73
#define SCLEX_CSOUND 74
#define SCLEX_FREEBASIC 75
#define SCLEX_INNOSETUP 76
#define SCLEX_OPAL 77
#define SCLEX_SPICE 78
#define SCLEX_D 79
#define SCLEX_CMAKE 80
#define SCLEX_GAP 81
#define SCLEX_PLM 82
#define SCLEX_PROGRESS 83
#define SCLEX_ABAQUS 84
#define SCLEX_ASYMPTOTE 85
#define SCLEX_R 86
#define SCLEX_MAGIK 87
#define SCLEX_POWERSHELL 88
#define SCLEX_MYSQL 89
#define SCLEX_PO 90
#define SCLEX_TAL 91
#define SCLEX_COBOL 92
#define SCLEX_TACL 93
#define SCLEX_SORCUS 94
#define SCLEX_POWERPRO 95
#define SCLEX_NIMROD 96
#define SCLEX_SML 97
#define SCLEX_MARKDOWN 98
#define SCLEX_AUTOMATIC 1000
#define SCE_P_DEFAULT 0
#define SCE_P_COMMENTLINE 1
#define SCE_P_NUMBER 2
#define SCE_P_STRING 3
#define SCE_P_CHARACTER 4
#define SCE_P_WORD 5
#define SCE_P_TRIPLE 6
#define SCE_P_TRIPLEDOUBLE 7
#define SCE_P_CLASSNAME 8
#define SCE_P_DEFNAME 9
#define SCE_P_OPERATOR 10
#define SCE_P_IDENTIFIER 11
#define SCE_P_COMMENTBLOCK 12
#define SCE_P_STRINGEOL 13
#define SCE_P_WORD2 14
#define SCE_P_DECORATOR 15
#define SCE_C_DEFAULT 0
#define SCE_C_COMMENT 1
#define SCE_C_COMMENTLINE 2
#define SCE_C_COMMENTDOC 3
#define SCE_C_NUMBER 4
#define SCE_C_WORD 5
#define SCE_C_STRING 6
#define SCE_C_CHARACTER 7
#define SCE_C_UUID 8
#define SCE_C_PREPROCESSOR 9
#define SCE_C_OPERATOR 10
#define SCE_C_IDENTIFIER 11
#define SCE_C_STRINGEOL 12
#define SCE_C_VERBATIM 13
#define SCE_C_REGEX 14
#define SCE_C_COMMENTLINEDOC 15
#define SCE_C_WORD2 16
#define SCE_C_COMMENTDOCKEYWORD 17
#define SCE_C_COMMENTDOCKEYWORDERROR 18
#define SCE_C_GLOBALCLASS 19
/* C::B begin */
#define SCE_C_WXSMITH 20
/* C::B end */
#define SCE_D_DEFAULT 0
#define SCE_D_COMMENT 1
#define SCE_D_COMMENTLINE 2
#define SCE_D_COMMENTDOC 3
#define SCE_D_COMMENTNESTED 4
#define SCE_D_NUMBER 5
#define SCE_D_WORD 6
#define SCE_D_WORD2 7
#define SCE_D_WORD3 8
#define SCE_D_TYPEDEF 9
#define SCE_D_STRING 10
#define SCE_D_STRINGEOL 11
#define SCE_D_CHARACTER 12
#define SCE_D_OPERATOR 13
#define SCE_D_IDENTIFIER 14
#define SCE_D_COMMENTLINEDOC 15
#define SCE_D_COMMENTDOCKEYWORD 16
#define SCE_D_COMMENTDOCKEYWORDERROR 17
#define SCE_D_STRINGB 18
#define SCE_D_STRINGR 19
#define SCE_D_WORD5 20
#define SCE_D_WORD6 21
#define SCE_D_WORD7 22
#define SCE_TCL_DEFAULT 0
#define SCE_TCL_COMMENT 1
#define SCE_TCL_COMMENTLINE 2
#define SCE_TCL_NUMBER 3
#define SCE_TCL_WORD_IN_QUOTE 4
#define SCE_TCL_IN_QUOTE 5
#define SCE_TCL_OPERATOR 6
#define SCE_TCL_IDENTIFIER 7
#define SCE_TCL_SUBSTITUTION 8
#define SCE_TCL_SUB_BRACE 9
#define SCE_TCL_MODIFIER 10
#define SCE_TCL_EXPAND 11
#define SCE_TCL_WORD 12
#define SCE_TCL_WORD2 13
#define SCE_TCL_WORD3 14
#define SCE_TCL_WORD4 15
#define SCE_TCL_WORD5 16
#define SCE_TCL_WORD6 17
#define SCE_TCL_WORD7 18
#define SCE_TCL_WORD8 19
#define SCE_TCL_COMMENT_BOX 20
#define SCE_TCL_BLOCK_COMMENT 21
#define SCE_H_DEFAULT 0
#define SCE_H_TAG 1
#define SCE_H_TAGUNKNOWN 2
#define SCE_H_ATTRIBUTE 3
#define SCE_H_ATTRIBUTEUNKNOWN 4
#define SCE_H_NUMBER 5
#define SCE_H_DOUBLESTRING 6
#define SCE_H_SINGLESTRING 7
#define SCE_H_OTHER 8
#define SCE_H_COMMENT 9
#define SCE_H_ENTITY 10
#define SCE_H_TAGEND 11
#define SCE_H_XMLSTART 12
#define SCE_H_XMLEND 13
#define SCE_H_SCRIPT 14
#define SCE_H_ASP 15
#define SCE_H_ASPAT 16
#define SCE_H_CDATA 17
#define SCE_H_QUESTION 18
#define SCE_H_VALUE 19
#define SCE_H_XCCOMMENT 20
#define SCE_H_SGML_DEFAULT 21
#define SCE_H_SGML_COMMAND 22
#define SCE_H_SGML_1ST_PARAM 23
#define SCE_H_SGML_DOUBLESTRING 24
#define SCE_H_SGML_SIMPLESTRING 25
#define SCE_H_SGML_ERROR 26
#define SCE_H_SGML_SPECIAL 27
#define SCE_H_SGML_ENTITY 28
#define SCE_H_SGML_COMMENT 29
#define SCE_H_SGML_1ST_PARAM_COMMENT 30
#define SCE_H_SGML_BLOCK_DEFAULT 31
#define SCE_HJ_START 40
#define SCE_HJ_DEFAULT 41
#define SCE_HJ_COMMENT 42
#define SCE_HJ_COMMENTLINE 43
#define SCE_HJ_COMMENTDOC 44
#define SCE_HJ_NUMBER 45
#define SCE_HJ_WORD 46
#define SCE_HJ_KEYWORD 47
#define SCE_HJ_DOUBLESTRING 48
#define SCE_HJ_SINGLESTRING 49
#define SCE_HJ_SYMBOLS 50
#define SCE_HJ_STRINGEOL 51
#define SCE_HJ_REGEX 52
#define SCE_HJA_START 55
#define SCE_HJA_DEFAULT 56
#define SCE_HJA_COMMENT 57
#define SCE_HJA_COMMENTLINE 58
#define SCE_HJA_COMMENTDOC 59
#define SCE_HJA_NUMBER 60
#define SCE_HJA_WORD 61
#define SCE_HJA_KEYWORD 62
#define SCE_HJA_DOUBLESTRING 63
#define SCE_HJA_SINGLESTRING 64
#define SCE_HJA_SYMBOLS 65
#define SCE_HJA_STRINGEOL 66
#define SCE_HJA_REGEX 67
#define SCE_HB_START 70
#define SCE_HB_DEFAULT 71
#define SCE_HB_COMMENTLINE 72
#define SCE_HB_NUMBER 73
#define SCE_HB_WORD 74
#define SCE_HB_STRING 75
#define SCE_HB_IDENTIFIER 76
#define SCE_HB_STRINGEOL 77
#define SCE_HBA_START 80
#define SCE_HBA_DEFAULT 81
#define SCE_HBA_COMMENTLINE 82
#define SCE_HBA_NUMBER 83
#define SCE_HBA_WORD 84
#define SCE_HBA_STRING 85
#define SCE_HBA_IDENTIFIER 86
#define SCE_HBA_STRINGEOL 87
#define SCE_HP_START 90
#define SCE_HP_DEFAULT 91
#define SCE_HP_COMMENTLINE 92
#define SCE_HP_NUMBER 93
#define SCE_HP_STRING 94
#define SCE_HP_CHARACTER 95
#define SCE_HP_WORD 96
#define SCE_HP_TRIPLE 97
#define SCE_HP_TRIPLEDOUBLE 98
#define SCE_HP_CLASSNAME 99
#define SCE_HP_DEFNAME 100
#define SCE_HP_OPERATOR 101
#define SCE_HP_IDENTIFIER 102
#define SCE_HPHP_COMPLEX_VARIABLE 104
#define SCE_HPA_START 105
#define SCE_HPA_DEFAULT 106
#define SCE_HPA_COMMENTLINE 107
#define SCE_HPA_NUMBER 108
#define SCE_HPA_STRING 109
#define SCE_HPA_CHARACTER 110
#define SCE_HPA_WORD 111
#define SCE_HPA_TRIPLE 112
#define SCE_HPA_TRIPLEDOUBLE 113
#define SCE_HPA_CLASSNAME 114
#define SCE_HPA_DEFNAME 115
#define SCE_HPA_OPERATOR 116
#define SCE_HPA_IDENTIFIER 117
#define SCE_HPHP_DEFAULT 118
#define SCE_HPHP_HSTRING 119
#define SCE_HPHP_SIMPLESTRING 120
#define SCE_HPHP_WORD 121
#define SCE_HPHP_NUMBER 122
#define SCE_HPHP_VARIABLE 123
#define SCE_HPHP_COMMENT 124
#define SCE_HPHP_COMMENTLINE 125
#define SCE_HPHP_HSTRING_VARIABLE 126
#define SCE_HPHP_OPERATOR 127
#define SCE_PL_DEFAULT 0
#define SCE_PL_ERROR 1
#define SCE_PL_COMMENTLINE 2
#define SCE_PL_POD 3
#define SCE_PL_NUMBER 4
#define SCE_PL_WORD 5
#define SCE_PL_STRING 6
#define SCE_PL_CHARACTER 7
#define SCE_PL_PUNCTUATION 8
#define SCE_PL_PREPROCESSOR 9
#define SCE_PL_OPERATOR 10
#define SCE_PL_IDENTIFIER 11
#define SCE_PL_SCALAR 12
#define SCE_PL_ARRAY 13
#define SCE_PL_HASH 14
#define SCE_PL_SYMBOLTABLE 15
#define SCE_PL_VARIABLE_INDEXER 16
#define SCE_PL_REGEX 17
#define SCE_PL_REGSUBST 18
#define SCE_PL_LONGQUOTE 19
#define SCE_PL_BACKTICKS 20
#define SCE_PL_DATASECTION 21
#define SCE_PL_HERE_DELIM 22
#define SCE_PL_HERE_Q 23
#define SCE_PL_HERE_QQ 24
#define SCE_PL_HERE_QX 25
#define SCE_PL_STRING_Q 26
#define SCE_PL_STRING_QQ 27
#define SCE_PL_STRING_QX 28
#define SCE_PL_STRING_QR 29
#define SCE_PL_STRING_QW 30
#define SCE_PL_POD_VERB 31
#define SCE_PL_SUB_PROTOTYPE 40
#define SCE_PL_FORMAT_IDENT 41
#define SCE_PL_FORMAT 42
#define SCE_RB_DEFAULT 0
#define SCE_RB_ERROR 1
#define SCE_RB_COMMENTLINE 2
#define SCE_RB_POD 3
#define SCE_RB_NUMBER 4
#define SCE_RB_WORD 5
#define SCE_RB_STRING 6
#define SCE_RB_CHARACTER 7
#define SCE_RB_CLASSNAME 8
#define SCE_RB_DEFNAME 9
#define SCE_RB_OPERATOR 10
#define SCE_RB_IDENTIFIER 11
#define SCE_RB_REGEX 12
#define SCE_RB_GLOBAL 13
#define SCE_RB_SYMBOL 14
#define SCE_RB_MODULE_NAME 15
#define SCE_RB_INSTANCE_VAR 16
#define SCE_RB_CLASS_VAR 17
#define SCE_RB_BACKTICKS 18
#define SCE_RB_DATASECTION 19
#define SCE_RB_HERE_DELIM 20
#define SCE_RB_HERE_Q 21
#define SCE_RB_HERE_QQ 22
#define SCE_RB_HERE_QX 23
#define SCE_RB_STRING_Q 24
#define SCE_RB_STRING_QQ 25
#define SCE_RB_STRING_QX 26
#define SCE_RB_STRING_QR 27
#define SCE_RB_STRING_QW 28
#define SCE_RB_WORD_DEMOTED 29
#define SCE_RB_STDIN 30
#define SCE_RB_STDOUT 31
#define SCE_RB_STDERR 40
#define SCE_RB_UPPER_BOUND 41
#define SCE_B_DEFAULT 0
#define SCE_B_COMMENT 1
#define SCE_B_NUMBER 2
#define SCE_B_KEYWORD 3
#define SCE_B_STRING 4
#define SCE_B_PREPROCESSOR 5
#define SCE_B_OPERATOR 6
#define SCE_B_IDENTIFIER 7
#define SCE_B_DATE 8
#define SCE_B_STRINGEOL 9
#define SCE_B_KEYWORD2 10
#define SCE_B_KEYWORD3 11
#define SCE_B_KEYWORD4 12
#define SCE_B_CONSTANT 13
#define SCE_B_ASM 14
#define SCE_B_LABEL 15
#define SCE_B_ERROR 16
#define SCE_B_HEXNUMBER 17
#define SCE_B_BINNUMBER 18
#define SCE_PROPS_DEFAULT 0
#define SCE_PROPS_COMMENT 1
#define SCE_PROPS_SECTION 2
#define SCE_PROPS_ASSIGNMENT 3
#define SCE_PROPS_DEFVAL 4
#define SCE_PROPS_KEY 5
#define SCE_L_DEFAULT 0
#define SCE_L_COMMAND 1
#define SCE_L_TAG 2
#define SCE_L_MATH 3
#define SCE_L_COMMENT 4
#define SCE_LUA_DEFAULT 0
#define SCE_LUA_COMMENT 1
#define SCE_LUA_COMMENTLINE 2
#define SCE_LUA_COMMENTDOC 3
#define SCE_LUA_NUMBER 4
#define SCE_LUA_WORD 5
#define SCE_LUA_STRING 6
#define SCE_LUA_CHARACTER 7
#define SCE_LUA_LITERALSTRING 8
#define SCE_LUA_PREPROCESSOR 9
#define SCE_LUA_OPERATOR 10
#define SCE_LUA_IDENTIFIER 11
#define SCE_LUA_STRINGEOL 12
#define SCE_LUA_WORD2 13
#define SCE_LUA_WORD3 14
#define SCE_LUA_WORD4 15
#define SCE_LUA_WORD5 16
#define SCE_LUA_WORD6 17
#define SCE_LUA_WORD7 18
#define SCE_LUA_WORD8 19
#define SCE_ERR_DEFAULT 0
#define SCE_ERR_PYTHON 1
#define SCE_ERR_GCC 2
#define SCE_ERR_MS 3
#define SCE_ERR_CMD 4
#define SCE_ERR_BORLAND 5
#define SCE_ERR_PERL 6
#define SCE_ERR_NET 7
#define SCE_ERR_LUA 8
#define SCE_ERR_CTAG 9
#define SCE_ERR_DIFF_CHANGED 10
#define SCE_ERR_DIFF_ADDITION 11
#define SCE_ERR_DIFF_DELETION 12
#define SCE_ERR_DIFF_MESSAGE 13
#define SCE_ERR_PHP 14
#define SCE_ERR_ELF 15
#define SCE_ERR_IFC 16
#define SCE_ERR_IFORT 17
#define SCE_ERR_ABSF 18
#define SCE_ERR_TIDY 19
#define SCE_ERR_JAVA_STACK 20
#define SCE_ERR_VALUE 21
#define SCE_BAT_DEFAULT 0
#define SCE_BAT_COMMENT 1
#define SCE_BAT_WORD 2
#define SCE_BAT_LABEL 3
#define SCE_BAT_HIDE 4
#define SCE_BAT_COMMAND 5
#define SCE_BAT_IDENTIFIER 6
#define SCE_BAT_OPERATOR 7
#define SCE_MAKE_DEFAULT 0
#define SCE_MAKE_COMMENT 1
#define SCE_MAKE_PREPROCESSOR 2
#define SCE_MAKE_IDENTIFIER 3
#define SCE_MAKE_OPERATOR 4
#define SCE_MAKE_TARGET 5
#define SCE_MAKE_IDEOL 9
#define SCE_DIFF_DEFAULT 0
#define SCE_DIFF_COMMENT 1
#define SCE_DIFF_COMMAND 2
#define SCE_DIFF_HEADER 3
#define SCE_DIFF_POSITION 4
#define SCE_DIFF_DELETED 5
#define SCE_DIFF_ADDED 6
#define SCE_DIFF_CHANGED 7
#define SCE_CONF_DEFAULT 0
#define SCE_CONF_COMMENT 1
#define SCE_CONF_NUMBER 2
#define SCE_CONF_IDENTIFIER 3
#define SCE_CONF_EXTENSION 4
#define SCE_CONF_PARAMETER 5
#define SCE_CONF_STRING 6
#define SCE_CONF_OPERATOR 7
#define SCE_CONF_IP 8
#define SCE_CONF_DIRECTIVE 9
#define SCE_AVE_DEFAULT 0
#define SCE_AVE_COMMENT 1
#define SCE_AVE_NUMBER 2
#define SCE_AVE_WORD 3
#define SCE_AVE_STRING 6
#define SCE_AVE_ENUM 7
#define SCE_AVE_STRINGEOL 8
#define SCE_AVE_IDENTIFIER 9
#define SCE_AVE_OPERATOR 10
#define SCE_AVE_WORD1 11
#define SCE_AVE_WORD2 12
#define SCE_AVE_WORD3 13
#define SCE_AVE_WORD4 14
#define SCE_AVE_WORD5 15
#define SCE_AVE_WORD6 16
#define SCE_ADA_DEFAULT 0
#define SCE_ADA_WORD 1
#define SCE_ADA_IDENTIFIER 2
#define SCE_ADA_NUMBER 3
#define SCE_ADA_DELIMITER 4
#define SCE_ADA_CHARACTER 5
#define SCE_ADA_CHARACTEREOL 6
#define SCE_ADA_STRING 7
#define SCE_ADA_STRINGEOL 8
#define SCE_ADA_LABEL 9
#define SCE_ADA_COMMENTLINE 10
#define SCE_ADA_ILLEGAL 11
#define SCE_BAAN_DEFAULT 0
#define SCE_BAAN_COMMENT 1
#define SCE_BAAN_COMMENTDOC 2
#define SCE_BAAN_NUMBER 3
#define SCE_BAAN_WORD 4
#define SCE_BAAN_STRING 5
#define SCE_BAAN_PREPROCESSOR 6
#define SCE_BAAN_OPERATOR 7
#define SCE_BAAN_IDENTIFIER 8
#define SCE_BAAN_STRINGEOL 9
#define SCE_BAAN_WORD2 10
#define SCE_LISP_DEFAULT 0
#define SCE_LISP_COMMENT 1
#define SCE_LISP_NUMBER 2
#define SCE_LISP_KEYWORD 3
#define SCE_LISP_KEYWORD_KW 4
#define SCE_LISP_SYMBOL 5
#define SCE_LISP_STRING 6
#define SCE_LISP_STRINGEOL 8
#define SCE_LISP_IDENTIFIER 9
#define SCE_LISP_OPERATOR 10
#define SCE_LISP_SPECIAL 11
#define SCE_LISP_MULTI_COMMENT 12
#define SCE_EIFFEL_DEFAULT 0
#define SCE_EIFFEL_COMMENTLINE 1
#define SCE_EIFFEL_NUMBER 2
#define SCE_EIFFEL_WORD 3
#define SCE_EIFFEL_STRING 4
#define SCE_EIFFEL_CHARACTER 5
#define SCE_EIFFEL_OPERATOR 6
#define SCE_EIFFEL_IDENTIFIER 7
#define SCE_EIFFEL_STRINGEOL 8
#define SCE_NNCRONTAB_DEFAULT 0
#define SCE_NNCRONTAB_COMMENT 1
#define SCE_NNCRONTAB_TASK 2
#define SCE_NNCRONTAB_SECTION 3
#define SCE_NNCRONTAB_KEYWORD 4
#define SCE_NNCRONTAB_MODIFIER 5
#define SCE_NNCRONTAB_ASTERISK 6
#define SCE_NNCRONTAB_NUMBER 7
#define SCE_NNCRONTAB_STRING 8
#define SCE_NNCRONTAB_ENVIRONMENT 9
#define SCE_NNCRONTAB_IDENTIFIER 10
#define SCE_FORTH_DEFAULT 0
#define SCE_FORTH_COMMENT 1
#define SCE_FORTH_COMMENT_ML 2
#define SCE_FORTH_IDENTIFIER 3
#define SCE_FORTH_CONTROL 4
#define SCE_FORTH_KEYWORD 5
#define SCE_FORTH_DEFWORD 6
#define SCE_FORTH_PREWORD1 7
#define SCE_FORTH_PREWORD2 8
#define SCE_FORTH_NUMBER 9
#define SCE_FORTH_STRING 10
#define SCE_FORTH_LOCALE 11
#define SCE_MATLAB_DEFAULT 0
#define SCE_MATLAB_COMMENT 1
#define SCE_MATLAB_COMMAND 2
#define SCE_MATLAB_NUMBER 3
#define SCE_MATLAB_KEYWORD 4
#define SCE_MATLAB_STRING 5
#define SCE_MATLAB_OPERATOR 6
#define SCE_MATLAB_IDENTIFIER 7
#define SCE_MATLAB_DOUBLEQUOTESTRING 8
#define SCE_SCRIPTOL_DEFAULT 0
#define SCE_SCRIPTOL_WHITE 1
#define SCE_SCRIPTOL_COMMENTLINE 2
#define SCE_SCRIPTOL_PERSISTENT 3
#define SCE_SCRIPTOL_CSTYLE 4
#define SCE_SCRIPTOL_COMMENTBLOCK 5
#define SCE_SCRIPTOL_NUMBER 6
#define SCE_SCRIPTOL_STRING 7
#define SCE_SCRIPTOL_CHARACTER 8
#define SCE_SCRIPTOL_STRINGEOL 9
#define SCE_SCRIPTOL_KEYWORD 10
#define SCE_SCRIPTOL_OPERATOR 11
#define SCE_SCRIPTOL_IDENTIFIER 12
#define SCE_SCRIPTOL_TRIPLE 13
#define SCE_SCRIPTOL_CLASSNAME 14
#define SCE_SCRIPTOL_PREPROCESSOR 15
#define SCE_ASM_DEFAULT 0
#define SCE_ASM_COMMENT 1
#define SCE_ASM_NUMBER 2
#define SCE_ASM_STRING 3
#define SCE_ASM_OPERATOR 4
#define SCE_ASM_IDENTIFIER 5
#define SCE_ASM_CPUINSTRUCTION 6
#define SCE_ASM_MATHINSTRUCTION 7
#define SCE_ASM_REGISTER 8
#define SCE_ASM_DIRECTIVE 9
#define SCE_ASM_DIRECTIVEOPERAND 10
#define SCE_ASM_COMMENTBLOCK 11
#define SCE_ASM_CHARACTER 12
#define SCE_ASM_STRINGEOL 13
#define SCE_ASM_EXTINSTRUCTION 14
#define SCE_F_DEFAULT 0
#define SCE_F_COMMENT 1
#define SCE_F_NUMBER 2
#define SCE_F_STRING1 3
#define SCE_F_STRING2 4
#define SCE_F_STRINGEOL 5
#define SCE_F_OPERATOR 6
#define SCE_F_IDENTIFIER 7
#define SCE_F_WORD 8
#define SCE_F_WORD2 9
#define SCE_F_WORD3 10
#define SCE_F_PREPROCESSOR 11
#define SCE_F_OPERATOR2 12
#define SCE_F_LABEL 13
#define SCE_F_CONTINUATION 14
#define SCE_CSS_DEFAULT 0
#define SCE_CSS_TAG 1
#define SCE_CSS_CLASS 2
#define SCE_CSS_PSEUDOCLASS 3
#define SCE_CSS_UNKNOWN_PSEUDOCLASS 4
#define SCE_CSS_OPERATOR 5
#define SCE_CSS_IDENTIFIER 6
#define SCE_CSS_UNKNOWN_IDENTIFIER 7
#define SCE_CSS_VALUE 8
#define SCE_CSS_COMMENT 9
#define SCE_CSS_ID 10
#define SCE_CSS_IMPORTANT 11
#define SCE_CSS_DIRECTIVE 12
#define SCE_CSS_DOUBLESTRING 13
#define SCE_CSS_SINGLESTRING 14
#define SCE_CSS_IDENTIFIER2 15
#define SCE_CSS_ATTRIBUTE 16
#define SCE_CSS_IDENTIFIER3 17
#define SCE_CSS_PSEUDOELEMENT 18
#define SCE_CSS_EXTENDED_IDENTIFIER 19
#define SCE_CSS_EXTENDED_PSEUDOCLASS 20
#define SCE_CSS_EXTENDED_PSEUDOELEMENT 21
#define SCE_POV_DEFAULT 0
#define SCE_POV_COMMENT 1
#define SCE_POV_COMMENTLINE 2
#define SCE_POV_NUMBER 3
#define SCE_POV_OPERATOR 4
#define SCE_POV_IDENTIFIER 5
#define SCE_POV_STRING 6
#define SCE_POV_STRINGEOL 7
#define SCE_POV_DIRECTIVE 8
#define SCE_POV_BADDIRECTIVE 9
#define SCE_POV_WORD2 10
#define SCE_POV_WORD3 11
#define SCE_POV_WORD4 12
#define SCE_POV_WORD5 13
#define SCE_POV_WORD6 14
#define SCE_POV_WORD7 15
#define SCE_POV_WORD8 16
#define SCE_LOUT_DEFAULT 0
#define SCE_LOUT_COMMENT 1
#define SCE_LOUT_NUMBER 2
#define SCE_LOUT_WORD 3
#define SCE_LOUT_WORD2 4
#define SCE_LOUT_WORD3 5
#define SCE_LOUT_WORD4 6
#define SCE_LOUT_STRING 7
#define SCE_LOUT_OPERATOR 8
#define SCE_LOUT_IDENTIFIER 9
#define SCE_LOUT_STRINGEOL 10
#define SCE_ESCRIPT_DEFAULT 0
#define SCE_ESCRIPT_COMMENT 1
#define SCE_ESCRIPT_COMMENTLINE 2
#define SCE_ESCRIPT_COMMENTDOC 3
#define SCE_ESCRIPT_NUMBER 4
#define SCE_ESCRIPT_WORD 5
#define SCE_ESCRIPT_STRING 6
#define SCE_ESCRIPT_OPERATOR 7
#define SCE_ESCRIPT_IDENTIFIER 8
#define SCE_ESCRIPT_BRACE 9
#define SCE_ESCRIPT_WORD2 10
#define SCE_ESCRIPT_WORD3 11
#define SCE_PS_DEFAULT 0
#define SCE_PS_COMMENT 1
#define SCE_PS_DSC_COMMENT 2
#define SCE_PS_DSC_VALUE 3
#define SCE_PS_NUMBER 4
#define SCE_PS_NAME 5
#define SCE_PS_KEYWORD 6
#define SCE_PS_LITERAL 7
#define SCE_PS_IMMEVAL 8
#define SCE_PS_PAREN_ARRAY 9
#define SCE_PS_PAREN_DICT 10
#define SCE_PS_PAREN_PROC 11
#define SCE_PS_TEXT 12
#define SCE_PS_HEXSTRING 13
#define SCE_PS_BASE85STRING 14
#define SCE_PS_BADSTRINGCHAR 15
#define SCE_NSIS_DEFAULT 0
#define SCE_NSIS_COMMENT 1
#define SCE_NSIS_STRINGDQ 2
#define SCE_NSIS_STRINGLQ 3
#define SCE_NSIS_STRINGRQ 4
#define SCE_NSIS_FUNCTION 5
#define SCE_NSIS_VARIABLE 6
#define SCE_NSIS_LABEL 7
#define SCE_NSIS_USERDEFINED 8
#define SCE_NSIS_SECTIONDEF 9
#define SCE_NSIS_SUBSECTIONDEF 10
#define SCE_NSIS_IFDEFINEDEF 11
#define SCE_NSIS_MACRODEF 12
#define SCE_NSIS_STRINGVAR 13
#define SCE_NSIS_NUMBER 14
#define SCE_NSIS_SECTIONGROUP 15
#define SCE_NSIS_PAGEEX 16
#define SCE_NSIS_FUNCTIONDEF 17
#define SCE_NSIS_COMMENTBOX 18
#define SCE_MMIXAL_LEADWS 0
#define SCE_MMIXAL_COMMENT 1
#define SCE_MMIXAL_LABEL 2
#define SCE_MMIXAL_OPCODE 3
#define SCE_MMIXAL_OPCODE_PRE 4
#define SCE_MMIXAL_OPCODE_VALID 5
#define SCE_MMIXAL_OPCODE_UNKNOWN 6
#define SCE_MMIXAL_OPCODE_POST 7
#define SCE_MMIXAL_OPERANDS 8
#define SCE_MMIXAL_NUMBER 9
#define SCE_MMIXAL_REF 10
#define SCE_MMIXAL_CHAR 11
#define SCE_MMIXAL_STRING 12
#define SCE_MMIXAL_REGISTER 13
#define SCE_MMIXAL_HEX 14
#define SCE_MMIXAL_OPERATOR 15
#define SCE_MMIXAL_SYMBOL 16
#define SCE_MMIXAL_INCLUDE 17
#define SCE_CLW_DEFAULT 0
#define SCE_CLW_LABEL 1
#define SCE_CLW_COMMENT 2
#define SCE_CLW_STRING 3
#define SCE_CLW_USER_IDENTIFIER 4
#define SCE_CLW_INTEGER_CONSTANT 5
#define SCE_CLW_REAL_CONSTANT 6
#define SCE_CLW_PICTURE_STRING 7
#define SCE_CLW_KEYWORD 8
#define SCE_CLW_COMPILER_DIRECTIVE 9
#define SCE_CLW_RUNTIME_EXPRESSIONS 10
#define SCE_CLW_BUILTIN_PROCEDURES_FUNCTION 11
#define SCE_CLW_STRUCTURE_DATA_TYPE 12
#define SCE_CLW_ATTRIBUTE 13
#define SCE_CLW_STANDARD_EQUATE 14
#define SCE_CLW_ERROR 15
#define SCE_CLW_DEPRECATED 16
#define SCE_LOT_DEFAULT 0
#define SCE_LOT_HEADER 1
#define SCE_LOT_BREAK 2
#define SCE_LOT_SET 3
#define SCE_LOT_PASS 4
#define SCE_LOT_FAIL 5
#define SCE_LOT_ABORT 6
#define SCE_YAML_DEFAULT 0
#define SCE_YAML_COMMENT 1
#define SCE_YAML_IDENTIFIER 2
#define SCE_YAML_KEYWORD 3
#define SCE_YAML_NUMBER 4
#define SCE_YAML_REFERENCE 5
#define SCE_YAML_DOCUMENT 6
#define SCE_YAML_TEXT 7
#define SCE_YAML_ERROR 8
#define SCE_YAML_OPERATOR 9
#define SCE_TEX_DEFAULT 0
#define SCE_TEX_SPECIAL 1
#define SCE_TEX_GROUP 2
#define SCE_TEX_SYMBOL 3
#define SCE_TEX_COMMAND 4
#define SCE_TEX_TEXT 5
#define SCE_METAPOST_DEFAULT 0
#define SCE_METAPOST_SPECIAL 1
#define SCE_METAPOST_GROUP 2
#define SCE_METAPOST_SYMBOL 3
#define SCE_METAPOST_COMMAND 4
#define SCE_METAPOST_TEXT 5
#define SCE_METAPOST_EXTRA 6
#define SCE_ERLANG_DEFAULT 0
#define SCE_ERLANG_COMMENT 1
#define SCE_ERLANG_VARIABLE 2
#define SCE_ERLANG_NUMBER 3
#define SCE_ERLANG_KEYWORD 4
#define SCE_ERLANG_STRING 5
#define SCE_ERLANG_OPERATOR 6
#define SCE_ERLANG_ATOM 7
#define SCE_ERLANG_FUNCTION_NAME 8
#define SCE_ERLANG_CHARACTER 9
#define SCE_ERLANG_MACRO 10
#define SCE_ERLANG_RECORD 11
#define SCE_ERLANG_SEPARATOR 12
#define SCE_ERLANG_NODE_NAME 13
#define SCE_ERLANG_UNKNOWN 31
#define SCE_MSSQL_DEFAULT 0
#define SCE_MSSQL_COMMENT 1
#define SCE_MSSQL_LINE_COMMENT 2
#define SCE_MSSQL_NUMBER 3
#define SCE_MSSQL_STRING 4
#define SCE_MSSQL_OPERATOR 5
#define SCE_MSSQL_IDENTIFIER 6
#define SCE_MSSQL_VARIABLE 7
#define SCE_MSSQL_COLUMN_NAME 8
#define SCE_MSSQL_STATEMENT 9
#define SCE_MSSQL_DATATYPE 10
#define SCE_MSSQL_SYSTABLE 11
#define SCE_MSSQL_GLOBAL_VARIABLE 12
#define SCE_MSSQL_FUNCTION 13
#define SCE_MSSQL_STORED_PROCEDURE 14
#define SCE_MSSQL_DEFAULT_PREF_DATATYPE 15
#define SCE_MSSQL_COLUMN_NAME_2 16
#define SCE_V_DEFAULT 0
#define SCE_V_COMMENT 1
#define SCE_V_COMMENTLINE 2
#define SCE_V_COMMENTLINEBANG 3
#define SCE_V_NUMBER 4
#define SCE_V_WORD 5
#define SCE_V_STRING 6
#define SCE_V_WORD2 7
#define SCE_V_WORD3 8
#define SCE_V_PREPROCESSOR 9
#define SCE_V_OPERATOR 10
#define SCE_V_IDENTIFIER 11
#define SCE_V_STRINGEOL 12
#define SCE_V_USER 19
#define SCE_KIX_DEFAULT 0
#define SCE_KIX_COMMENT 1
#define SCE_KIX_STRING1 2
#define SCE_KIX_STRING2 3
#define SCE_KIX_NUMBER 4
#define SCE_KIX_VAR 5
#define SCE_KIX_MACRO 6
#define SCE_KIX_KEYWORD 7
#define SCE_KIX_FUNCTIONS 8
#define SCE_KIX_OPERATOR 9
#define SCE_KIX_IDENTIFIER 31
#define SCE_GC_DEFAULT 0
#define SCE_GC_COMMENTLINE 1
#define SCE_GC_COMMENTBLOCK 2
#define SCE_GC_GLOBAL 3
#define SCE_GC_EVENT 4
#define SCE_GC_ATTRIBUTE 5
#define SCE_GC_CONTROL 6
#define SCE_GC_COMMAND 7
#define SCE_GC_STRING 8
#define SCE_GC_OPERATOR 9
#define SCE_SN_DEFAULT 0
#define SCE_SN_CODE 1
#define SCE_SN_COMMENTLINE 2
#define SCE_SN_COMMENTLINEBANG 3
#define SCE_SN_NUMBER 4
#define SCE_SN_WORD 5
#define SCE_SN_STRING 6
#define SCE_SN_WORD2 7
#define SCE_SN_WORD3 8
#define SCE_SN_PREPROCESSOR 9
#define SCE_SN_OPERATOR 10
#define SCE_SN_IDENTIFIER 11
#define SCE_SN_STRINGEOL 12
#define SCE_SN_REGEXTAG 13
#define SCE_SN_SIGNAL 14
#define SCE_SN_USER 19
#define SCE_AU3_DEFAULT 0
#define SCE_AU3_COMMENT 1
#define SCE_AU3_COMMENTBLOCK 2
#define SCE_AU3_NUMBER 3
#define SCE_AU3_FUNCTION 4
#define SCE_AU3_KEYWORD 5
#define SCE_AU3_MACRO 6
#define SCE_AU3_STRING 7
#define SCE_AU3_OPERATOR 8
#define SCE_AU3_VARIABLE 9
#define SCE_AU3_SENT 10
#define SCE_AU3_PREPROCESSOR 11
#define SCE_AU3_SPECIAL 12
#define SCE_AU3_EXPAND 13
#define SCE_AU3_COMOBJ 14
#define SCE_AU3_UDF 15
#define SCE_APDL_DEFAULT 0
#define SCE_APDL_COMMENT 1
#define SCE_APDL_COMMENTBLOCK 2
#define SCE_APDL_NUMBER 3
#define SCE_APDL_STRING 4
#define SCE_APDL_OPERATOR 5
#define SCE_APDL_WORD 6
#define SCE_APDL_PROCESSOR 7
#define SCE_APDL_COMMAND 8
#define SCE_APDL_SLASHCOMMAND 9
#define SCE_APDL_STARCOMMAND 10
#define SCE_APDL_ARGUMENT 11
#define SCE_APDL_FUNCTION 12
#define SCE_SH_DEFAULT 0
#define SCE_SH_ERROR 1
#define SCE_SH_COMMENTLINE 2
#define SCE_SH_NUMBER 3
#define SCE_SH_WORD 4
#define SCE_SH_STRING 5
#define SCE_SH_CHARACTER 6
#define SCE_SH_OPERATOR 7
#define SCE_SH_IDENTIFIER 8
#define SCE_SH_SCALAR 9
#define SCE_SH_PARAM 10
#define SCE_SH_BACKTICKS 11
#define SCE_SH_HERE_DELIM 12
#define SCE_SH_HERE_Q 13
#define SCE_ASN1_DEFAULT 0
#define SCE_ASN1_COMMENT 1
#define SCE_ASN1_IDENTIFIER 2
#define SCE_ASN1_STRING 3
#define SCE_ASN1_OID 4
#define SCE_ASN1_SCALAR 5
#define SCE_ASN1_KEYWORD 6
#define SCE_ASN1_ATTRIBUTE 7
#define SCE_ASN1_DESCRIPTOR 8
#define SCE_ASN1_TYPE 9
#define SCE_ASN1_OPERATOR 10
#define SCE_VHDL_DEFAULT 0
#define SCE_VHDL_COMMENT 1
#define SCE_VHDL_COMMENTLINEBANG 2
#define SCE_VHDL_NUMBER 3
#define SCE_VHDL_STRING 4
#define SCE_VHDL_OPERATOR 5
#define SCE_VHDL_IDENTIFIER 6
#define SCE_VHDL_STRINGEOL 7
#define SCE_VHDL_KEYWORD 8
#define SCE_VHDL_STDOPERATOR 9
#define SCE_VHDL_ATTRIBUTE 10
#define SCE_VHDL_STDFUNCTION 11
#define SCE_VHDL_STDPACKAGE 12
#define SCE_VHDL_STDTYPE 13
#define SCE_VHDL_USERWORD 14
#define SCE_CAML_DEFAULT 0
#define SCE_CAML_IDENTIFIER 1
#define SCE_CAML_TAGNAME 2
#define SCE_CAML_KEYWORD 3
#define SCE_CAML_KEYWORD2 4
#define SCE_CAML_KEYWORD3 5
#define SCE_CAML_LINENUM 6
#define SCE_CAML_OPERATOR 7
#define SCE_CAML_NUMBER 8
#define SCE_CAML_CHAR 9
#define SCE_CAML_WHITE 10
#define SCE_CAML_STRING 11
#define SCE_CAML_COMMENT 12
#define SCE_CAML_COMMENT1 13
#define SCE_CAML_COMMENT2 14
#define SCE_CAML_COMMENT3 15
#define SCE_HA_DEFAULT 0
#define SCE_HA_IDENTIFIER 1
#define SCE_HA_KEYWORD 2
#define SCE_HA_NUMBER 3
#define SCE_HA_STRING 4
#define SCE_HA_CHARACTER 5
#define SCE_HA_CLASS 6
#define SCE_HA_MODULE 7
#define SCE_HA_CAPITAL 8
#define SCE_HA_DATA 9
#define SCE_HA_IMPORT 10
#define SCE_HA_OPERATOR 11
#define SCE_HA_INSTANCE 12
#define SCE_HA_COMMENTLINE 13
#define SCE_HA_COMMENTBLOCK 14
#define SCE_HA_COMMENTBLOCK2 15
#define SCE_HA_COMMENTBLOCK3 16
#define SCE_T3_DEFAULT 0
#define SCE_T3_X_DEFAULT 1
#define SCE_T3_PREPROCESSOR 2
#define SCE_T3_BLOCK_COMMENT 3
#define SCE_T3_LINE_COMMENT 4
#define SCE_T3_OPERATOR 5
#define SCE_T3_KEYWORD 6
#define SCE_T3_NUMBER 7
#define SCE_T3_IDENTIFIER 8
#define SCE_T3_S_STRING 9
#define SCE_T3_D_STRING 10
#define SCE_T3_X_STRING 11
#define SCE_T3_LIB_DIRECTIVE 12
#define SCE_T3_MSG_PARAM 13
#define SCE_T3_HTML_TAG 14
#define SCE_T3_HTML_DEFAULT 15
#define SCE_T3_HTML_STRING 16
#define SCE_T3_USER1 17
#define SCE_T3_USER2 18
#define SCE_T3_USER3 19
#define SCE_T3_BRACE 20
#define SCE_REBOL_DEFAULT 0
#define SCE_REBOL_COMMENTLINE 1
#define SCE_REBOL_COMMENTBLOCK 2
#define SCE_REBOL_PREFACE 3
#define SCE_REBOL_OPERATOR 4
#define SCE_REBOL_CHARACTER 5
#define SCE_REBOL_QUOTEDSTRING 6
#define SCE_REBOL_BRACEDSTRING 7
#define SCE_REBOL_NUMBER 8
#define SCE_REBOL_PAIR 9
#define SCE_REBOL_TUPLE 10
#define SCE_REBOL_BINARY 11
#define SCE_REBOL_MONEY 12
#define SCE_REBOL_ISSUE 13
#define SCE_REBOL_TAG 14
#define SCE_REBOL_FILE 15
#define SCE_REBOL_EMAIL 16
#define SCE_REBOL_URL 17
#define SCE_REBOL_DATE 18
#define SCE_REBOL_TIME 19
#define SCE_REBOL_IDENTIFIER 20
#define SCE_REBOL_WORD 21
#define SCE_REBOL_WORD2 22
#define SCE_REBOL_WORD3 23
#define SCE_REBOL_WORD4 24
#define SCE_REBOL_WORD5 25
#define SCE_REBOL_WORD6 26
#define SCE_REBOL_WORD7 27
#define SCE_REBOL_WORD8 28
#define SCE_SQL_DEFAULT 0
#define SCE_SQL_COMMENT 1
#define SCE_SQL_COMMENTLINE 2
#define SCE_SQL_COMMENTDOC 3
#define SCE_SQL_NUMBER 4
#define SCE_SQL_WORD 5
#define SCE_SQL_STRING 6
#define SCE_SQL_CHARACTER 7
#define SCE_SQL_SQLPLUS 8
#define SCE_SQL_SQLPLUS_PROMPT 9
#define SCE_SQL_OPERATOR 10
#define SCE_SQL_IDENTIFIER 11
#define SCE_SQL_SQLPLUS_COMMENT 13
#define SCE_SQL_COMMENTLINEDOC 15
#define SCE_SQL_WORD2 16
#define SCE_SQL_COMMENTDOCKEYWORD 17
#define SCE_SQL_COMMENTDOCKEYWORDERROR 18
#define SCE_SQL_USER1 19
#define SCE_SQL_USER2 20
#define SCE_SQL_USER3 21
#define SCE_SQL_USER4 22
#define SCE_SQL_QUOTEDIDENTIFIER 23
#define SCE_ST_DEFAULT 0
#define SCE_ST_STRING 1
#define SCE_ST_NUMBER 2
#define SCE_ST_COMMENT 3
#define SCE_ST_SYMBOL 4
#define SCE_ST_BINARY 5
#define SCE_ST_BOOL 6
#define SCE_ST_SELF 7
#define SCE_ST_SUPER 8
#define SCE_ST_NIL 9
#define SCE_ST_GLOBAL 10
#define SCE_ST_RETURN 11
#define SCE_ST_SPECIAL 12
#define SCE_ST_KWSEND 13
#define SCE_ST_ASSIGN 14
#define SCE_ST_CHARACTER 15
#define SCE_ST_SPEC_SEL 16
#define SCE_FS_DEFAULT 0
#define SCE_FS_COMMENT 1
#define SCE_FS_COMMENTLINE 2
#define SCE_FS_COMMENTDOC 3
#define SCE_FS_COMMENTLINEDOC 4
#define SCE_FS_COMMENTDOCKEYWORD 5
#define SCE_FS_COMMENTDOCKEYWORDERROR 6
#define SCE_FS_KEYWORD 7
#define SCE_FS_KEYWORD2 8
#define SCE_FS_KEYWORD3 9
#define SCE_FS_KEYWORD4 10
#define SCE_FS_NUMBER 11
#define SCE_FS_STRING 12
#define SCE_FS_PREPROCESSOR 13
#define SCE_FS_OPERATOR 14
#define SCE_FS_IDENTIFIER 15
#define SCE_FS_DATE 16
#define SCE_FS_STRINGEOL 17
#define SCE_FS_CONSTANT 18
#define SCE_FS_ASM 19
#define SCE_FS_LABEL 20
#define SCE_FS_ERROR 21
#define SCE_FS_HEXNUMBER 22
#define SCE_FS_BINNUMBER 23
#define SCE_CSOUND_DEFAULT 0
#define SCE_CSOUND_COMMENT 1
#define SCE_CSOUND_NUMBER 2
#define SCE_CSOUND_OPERATOR 3
#define SCE_CSOUND_INSTR 4
#define SCE_CSOUND_IDENTIFIER 5
#define SCE_CSOUND_OPCODE 6
#define SCE_CSOUND_HEADERSTMT 7
#define SCE_CSOUND_USERKEYWORD 8
#define SCE_CSOUND_COMMENTBLOCK 9
#define SCE_CSOUND_PARAM 10
#define SCE_CSOUND_ARATE_VAR 11
#define SCE_CSOUND_KRATE_VAR 12
#define SCE_CSOUND_IRATE_VAR 13
#define SCE_CSOUND_GLOBAL_VAR 14
#define SCE_CSOUND_STRINGEOL 15
#define SCE_INNO_DEFAULT 0
#define SCE_INNO_COMMENT 1
#define SCE_INNO_KEYWORD 2
#define SCE_INNO_PARAMETER 3
#define SCE_INNO_SECTION 4
#define SCE_INNO_PREPROC 5
#define SCE_INNO_PREPROC_INLINE 6
#define SCE_INNO_INLINE_EXPANSION 6
#define SCE_INNO_COMMENT_PASCAL 7
#define SCE_INNO_KEYWORD_PASCAL 8
#define SCE_INNO_KEYWORD_USER 9
#define SCE_INNO_STRING_DOUBLE 10
#define SCE_INNO_STRING_SINGLE 11
#define SCE_INNO_IDENTIFIER 12
#define SCE_OPAL_SPACE 0
#define SCE_OPAL_COMMENT_BLOCK 1
#define SCE_OPAL_COMMENT_LINE 2
#define SCE_OPAL_INTEGER 3
#define SCE_OPAL_KEYWORD 4
#define SCE_OPAL_SORT 5
#define SCE_OPAL_STRING 6
#define SCE_OPAL_PAR 7
#define SCE_OPAL_BOOL_CONST 8
#define SCE_OPAL_DEFAULT 32
#define SCE_SPICE_DEFAULT 0
#define SCE_SPICE_IDENTIFIER 1
#define SCE_SPICE_KEYWORD 2
#define SCE_SPICE_KEYWORD2 3
#define SCE_SPICE_KEYWORD3 4
#define SCE_SPICE_NUMBER 5
#define SCE_SPICE_DELIMITER 6
#define SCE_SPICE_VALUE 7
#define SCE_SPICE_COMMENTLINE 8
#define SCE_CMAKE_DEFAULT 0
#define SCE_CMAKE_COMMENT 1
#define SCE_CMAKE_STRINGDQ 2
#define SCE_CMAKE_STRINGLQ 3
#define SCE_CMAKE_STRINGRQ 4
#define SCE_CMAKE_COMMANDS 5
#define SCE_CMAKE_PARAMETERS 6
#define SCE_CMAKE_VARIABLE 7
#define SCE_CMAKE_USERDEFINED 8
#define SCE_CMAKE_WHILEDEF 9
#define SCE_CMAKE_FOREACHDEF 10
#define SCE_CMAKE_IFDEFINEDEF 11
#define SCE_CMAKE_MACRODEF 12
#define SCE_CMAKE_STRINGVAR 13
#define SCE_CMAKE_NUMBER 14
#define SCE_GAP_DEFAULT 0
#define SCE_GAP_IDENTIFIER 1
#define SCE_GAP_KEYWORD 2
#define SCE_GAP_KEYWORD2 3
#define SCE_GAP_KEYWORD3 4
#define SCE_GAP_KEYWORD4 5
#define SCE_GAP_STRING 6
#define SCE_GAP_CHAR 7
#define SCE_GAP_OPERATOR 8
#define SCE_GAP_COMMENT 9
#define SCE_GAP_NUMBER 10
#define SCE_GAP_STRINGEOL 11
#define SCE_PLM_DEFAULT 0
#define SCE_PLM_COMMENT 1
#define SCE_PLM_STRING 2
#define SCE_PLM_NUMBER 3
#define SCE_PLM_IDENTIFIER 4
#define SCE_PLM_OPERATOR 5
#define SCE_PLM_CONTROL 6
#define SCE_PLM_KEYWORD 7
#define SCE_4GL_DEFAULT 0
#define SCE_4GL_NUMBER 1
#define SCE_4GL_WORD 2
#define SCE_4GL_STRING 3
#define SCE_4GL_CHARACTER 4
#define SCE_4GL_PREPROCESSOR 5
#define SCE_4GL_OPERATOR 6
#define SCE_4GL_IDENTIFIER 7
#define SCE_4GL_BLOCK 8
#define SCE_4GL_END 9
#define SCE_4GL_COMMENT1 10
#define SCE_4GL_COMMENT2 11
#define SCE_4GL_COMMENT3 12
#define SCE_4GL_COMMENT4 13
#define SCE_4GL_COMMENT5 14
#define SCE_4GL_COMMENT6 15
#define SCE_4GL_DEFAULT_ 16
#define SCE_4GL_NUMBER_ 17
#define SCE_4GL_WORD_ 18
#define SCE_4GL_STRING_ 19
#define SCE_4GL_CHARACTER_ 20
#define SCE_4GL_PREPROCESSOR_ 21
#define SCE_4GL_OPERATOR_ 22
#define SCE_4GL_IDENTIFIER_ 23
#define SCE_4GL_BLOCK_ 24
#define SCE_4GL_END_ 25
#define SCE_4GL_COMMENT1_ 26
#define SCE_4GL_COMMENT2_ 27
#define SCE_4GL_COMMENT3_ 28
#define SCE_4GL_COMMENT4_ 29
#define SCE_4GL_COMMENT5_ 30
#define SCE_4GL_COMMENT6_ 31
#define SCE_ABAQUS_DEFAULT 0
#define SCE_ABAQUS_COMMENT 1
#define SCE_ABAQUS_COMMENTBLOCK 2
#define SCE_ABAQUS_NUMBER 3
#define SCE_ABAQUS_STRING 4
#define SCE_ABAQUS_OPERATOR 5
#define SCE_ABAQUS_WORD 6
#define SCE_ABAQUS_PROCESSOR 7
#define SCE_ABAQUS_COMMAND 8
#define SCE_ABAQUS_SLASHCOMMAND 9
#define SCE_ABAQUS_STARCOMMAND 10
#define SCE_ABAQUS_ARGUMENT 11
#define SCE_ABAQUS_FUNCTION 12
#define SCE_ASY_DEFAULT 0
#define SCE_ASY_COMMENT 1
#define SCE_ASY_COMMENTLINE 2
#define SCE_ASY_NUMBER 3
#define SCE_ASY_WORD 4
#define SCE_ASY_STRING 5
#define SCE_ASY_CHARACTER 6
#define SCE_ASY_OPERATOR 7
#define SCE_ASY_IDENTIFIER 8
#define SCE_ASY_STRINGEOL 9
#define SCE_ASY_COMMENTLINEDOC 10
#define SCE_ASY_WORD2 11
#define SCE_R_DEFAULT 0
#define SCE_R_COMMENT 1
#define SCE_R_KWORD 2
#define SCE_R_BASEKWORD 3
#define SCE_R_OTHERKWORD 4
#define SCE_R_NUMBER 5
#define SCE_R_STRING 6
#define SCE_R_STRING2 7
#define SCE_R_OPERATOR 8
#define SCE_R_IDENTIFIER 9
#define SCE_R_INFIX 10
#define SCE_R_INFIXEOL 11
#define SCE_MAGIK_DEFAULT 0
#define SCE_MAGIK_COMMENT 1
#define SCE_MAGIK_HYPER_COMMENT 16
#define SCE_MAGIK_STRING 2
#define SCE_MAGIK_CHARACTER 3
#define SCE_MAGIK_NUMBER 4
#define SCE_MAGIK_IDENTIFIER 5
#define SCE_MAGIK_OPERATOR 6
#define SCE_MAGIK_FLOW 7
#define SCE_MAGIK_CONTAINER 8
#define SCE_MAGIK_BRACKET_BLOCK 9
#define SCE_MAGIK_BRACE_BLOCK 10
#define SCE_MAGIK_SQBRACKET_BLOCK 11
#define SCE_MAGIK_UNKNOWN_KEYWORD 12
#define SCE_MAGIK_KEYWORD 13
#define SCE_MAGIK_PRAGMA 14
#define SCE_MAGIK_SYMBOL 15
#define SCE_POWERSHELL_DEFAULT 0
#define SCE_POWERSHELL_COMMENT 1
#define SCE_POWERSHELL_STRING 2
#define SCE_POWERSHELL_CHARACTER 3
#define SCE_POWERSHELL_NUMBER 4
#define SCE_POWERSHELL_VARIABLE 5
#define SCE_POWERSHELL_OPERATOR 6
#define SCE_POWERSHELL_IDENTIFIER 7
#define SCE_POWERSHELL_KEYWORD 8
#define SCE_POWERSHELL_CMDLET 9
#define SCE_POWERSHELL_ALIAS 10
#define SCE_MYSQL_DEFAULT 0
#define SCE_MYSQL_COMMENT 1
#define SCE_MYSQL_COMMENTLINE 2
#define SCE_MYSQL_VARIABLE 3
#define SCE_MYSQL_SYSTEMVARIABLE 4
#define SCE_MYSQL_KNOWNSYSTEMVARIABLE 5
#define SCE_MYSQL_NUMBER 6
#define SCE_MYSQL_MAJORKEYWORD 7
#define SCE_MYSQL_KEYWORD 8
#define SCE_MYSQL_DATABASEOBJECT 9
#define SCE_MYSQL_PROCEDUREKEYWORD 10
#define SCE_MYSQL_STRING 11
#define SCE_MYSQL_SQSTRING 12
#define SCE_MYSQL_DQSTRING 13
#define SCE_MYSQL_OPERATOR 14
#define SCE_MYSQL_FUNCTION 15
#define SCE_MYSQL_IDENTIFIER 16
#define SCE_MYSQL_QUOTEDIDENTIFIER 17
#define SCE_MYSQL_USER1 18
#define SCE_MYSQL_USER2 19
#define SCE_MYSQL_USER3 20
#define SCE_MYSQL_HIDDENCOMMAND 21
#define SCE_PO_DEFAULT 0
#define SCE_PO_COMMENT 1
#define SCE_PO_MSGID 2
#define SCE_PO_MSGID_TEXT 3
#define SCE_PO_MSGSTR 4
#define SCE_PO_MSGSTR_TEXT 5
#define SCE_PO_MSGCTXT 6
#define SCE_PO_MSGCTXT_TEXT 7
#define SCE_PO_FUZZY 8
#define SCE_PAS_DEFAULT 0
#define SCE_PAS_IDENTIFIER 1
#define SCE_PAS_COMMENT 2
#define SCE_PAS_COMMENT2 3
#define SCE_PAS_COMMENTLINE 4
#define SCE_PAS_PREPROCESSOR 5
#define SCE_PAS_PREPROCESSOR2 6
#define SCE_PAS_NUMBER 7
#define SCE_PAS_HEXNUMBER 8
#define SCE_PAS_WORD 9
#define SCE_PAS_STRING 10
#define SCE_PAS_STRINGEOL 11
#define SCE_PAS_CHARACTER 12
#define SCE_PAS_OPERATOR 13
#define SCE_PAS_ASM 14
#define SCE_SORCUS_DEFAULT 0
#define SCE_SORCUS_COMMAND 1
#define SCE_SORCUS_PARAMETER 2
#define SCE_SORCUS_COMMENTLINE 3
#define SCE_SORCUS_STRING 4
#define SCE_SORCUS_STRINGEOL 5
#define SCE_SORCUS_IDENTIFIER 6
#define SCE_SORCUS_OPERATOR 7
#define SCE_SORCUS_NUMBER 8
#define SCE_SORCUS_CONSTANT 9
#define SCE_POWERPRO_DEFAULT 0
#define SCE_POWERPRO_COMMENTBLOCK 1
#define SCE_POWERPRO_COMMENTLINE 2
#define SCE_POWERPRO_NUMBER 3
#define SCE_POWERPRO_WORD 4
#define SCE_POWERPRO_WORD2 5
#define SCE_POWERPRO_WORD3 6
#define SCE_POWERPRO_WORD4 7
#define SCE_POWERPRO_DOUBLEQUOTEDSTRING 8
#define SCE_POWERPRO_SINGLEQUOTEDSTRING 9
#define SCE_POWERPRO_LINECONTINUE 10
#define SCE_POWERPRO_OPERATOR 11
#define SCE_POWERPRO_IDENTIFIER 12
#define SCE_POWERPRO_STRINGEOL 13
#define SCE_POWERPRO_VERBATIM 14
#define SCE_POWERPRO_ALTQUOTE 15
#define SCE_POWERPRO_FUNCTION 16
#define SCE_SML_DEFAULT 0
#define SCE_SML_IDENTIFIER 1
#define SCE_SML_TAGNAME 2
#define SCE_SML_KEYWORD 3
#define SCE_SML_KEYWORD2 4
#define SCE_SML_KEYWORD3 5
#define SCE_SML_LINENUM 6
#define SCE_SML_OPERATOR 7
#define SCE_SML_NUMBER 8
#define SCE_SML_CHAR 9
#define SCE_SML_STRING 11
#define SCE_SML_COMMENT 12
#define SCE_SML_COMMENT1 13
#define SCE_SML_COMMENT2 14
#define SCE_SML_COMMENT3 15
#define SCE_MARKDOWN_DEFAULT 0
#define SCE_MARKDOWN_LINE_BEGIN 1
#define SCE_MARKDOWN_STRONG1 2
#define SCE_MARKDOWN_STRONG2 3
#define SCE_MARKDOWN_EM1 4
#define SCE_MARKDOWN_EM2 5
#define SCE_MARKDOWN_HEADER1 6
#define SCE_MARKDOWN_HEADER2 7
#define SCE_MARKDOWN_HEADER3 8
#define SCE_MARKDOWN_HEADER4 9
#define SCE_MARKDOWN_HEADER5 10
#define SCE_MARKDOWN_HEADER6 11
#define SCE_MARKDOWN_PRECHAR 12
#define SCE_MARKDOWN_ULIST_ITEM 13
#define SCE_MARKDOWN_OLIST_ITEM 14
#define SCE_MARKDOWN_BLOCKQUOTE 15
#define SCE_MARKDOWN_STRIKEOUT 16
#define SCE_MARKDOWN_HRULE 17
#define SCE_MARKDOWN_LINK 18
#define SCE_MARKDOWN_CODE 19
#define SCE_MARKDOWN_CODE2 20
#define SCE_MARKDOWN_CODEBK 21
/* --Autogenerated -- end of section automatically generated from Scintilla.iface */

#endif
