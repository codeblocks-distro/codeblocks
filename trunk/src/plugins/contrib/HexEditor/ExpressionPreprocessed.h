/*
* This file is part of HexEditor plugin for Code::Blocks Studio
* Copyright (C) 2008 Bartlomiej Swiecki
*
* HexEditor plugin is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* HexEditor pluging is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmith. If not, see <http://www.gnu.org/licenses/>.
*
* $Revision:$
* $Id:$
* $HeadURL:$
*/

#ifndef EXPRESSIONPREPROCESSED_H
#define EXPRESSIONPREPROCESSED_H

#include <vector>
#include <wx/string.h>

namespace Expression
{
    /** \brief Errors which may occur on the execution */
    enum executionError
    {
        executedSuccessfully,
        errorArgumentIndex,
        errorOperationIndex,
        errorStackIndex,
        errorContentIndex,
        errorOperation,
        errorType,
        errorScript
    };

    class Value
    {
        public:

            Value( signed char v        ) { SetSignedInt( v ); }
            Value( signed short v       ) { SetSignedInt( v ); }
            Value( signed int v         ) { SetSignedInt( v ); }
            Value( signed long v        ) { SetSignedInt( v ); }
            Value( signed long long v   ) { SetSignedInt( v ); }
            Value( unsigned char v      ) { SetUnsignedInt( v ); }
            Value( unsigned short v     ) { SetUnsignedInt( v ); }
            Value( unsigned int v       ) { SetUnsignedInt( v ); }
            Value( unsigned long v      ) { SetUnsignedInt( v ); }
            Value( unsigned long long v ) { SetUnsignedInt( v ); }
            Value( float v              ) { SetFloat( v ); }
            Value( double v             ) { SetFloat( v ); }
            Value( long double v        ) { SetFloat( v ); }

            inline bool IsSignedInt()   { return m_Type == tSignedInt; }
            inline bool IsUnsignedInt() { return m_Type == tUnsignedInt; }
            inline bool IsFloat()       { return m_Type == tFloat; }

            inline signed long long   GetSignedInt()   { if ( !IsSignedInt()   ) throw errorType; return m_SignedInt; }
            inline unsigned long long GetUnsignedInt() { if ( !IsUnsignedInt() ) throw errorType; return m_UnsignedInt; }
            inline long double        GetFloat()       { if ( !IsFloat()       ) throw errorType; return m_Float; }

        private:

            enum TypeT
            {
                tSignedInt,
                tUnsignedInt,
                tFloat
            };

            TypeT m_Type;

            union
            {
                signed   long long m_SignedInt;
                unsigned long long m_UnsignedInt;
                long double        m_Float;
            };

            template< typename T > inline void SetSignedInt( T v )
            {
                m_Type = tSignedInt;
                m_SignedInt = v;
            }

            template< typename T > inline void SetUnsignedInt( T v )
            {
                m_Type = tUnsignedInt;
                m_SignedInt = v;
            }

            template< typename T > inline void SetFloat( T v )
            {
                m_Type = tFloat;
                m_Float = v;
            }
    };

    struct Operation
    {
        // Operation's code
        enum opCode
        {
            //Notyfi aboud the end of the script
            endScript = 0,

            // Push "current" address onto stack top modified with const argument
            pushCurrent,

            // load value from memory at address given at the stack top
            // and push the result back onto the stack
            loadMem,

            // get address from const argument,
            // read proper value from the memory and push it onto the stack
            // value is loaded from the code arguments array
            loadArg,

            // Simple arithmetic operations, pops two operands from stack top
            // and push the result onto the stack
            add,
            mul,
            div,
            mod,

            // Unary operators, pops operand from stack top and push the result
            neg,
            conv
        };

        // Argument modifiers
        enum modifier
        {
            modNone,
            modArg,
            modChar,
            modByte,
            modShort,
            modWord,
            modLong,
            modDword,
            modLongLong,
            modQword,
            modFloat,
            modDouble,
            modLongDouble,
        };

        unsigned m_OpCode: 4;
        unsigned m_Mod1:   4;
        unsigned m_Mod2:   4;

        short m_ConstArgument;
    };


    /** \brief Preprocessed expression */
    class Preprocessed
    {
        public:

            /** \brief Ctor */
            Preprocessed();

            /** \brief Dctor */
            ~Preprocessed();

            /** \brief Push operation onto the end of code */
            inline int PushOperation( const Operation& op )
            {
                m_Code.push_back( op );
                return (int)m_Code.size()-1;
            }

            /** \brief Push argumet onto the end of constant arguments list */
            inline int PushArgument( const Value& v )
            {
                m_CodeArguments.push_back( v );
                return (int)m_CodeArguments.size()-1;
            }

            /** \brief Get operation at given position */
            inline const Operation& GetOperation( int pos ) const
            {
                if ( (size_t)pos >= m_Code.size() ) throw errorOperationIndex;
                return m_Code[ pos ];
            }

            /** \brief Get argument at given position */
            inline const Value& GetArgument( int pos ) const
            {
                if ( (size_t)pos >= m_CodeArguments.size() ) throw errorArgumentIndex;
                return m_CodeArguments[ pos ];
            }

            /** \brief Dump the code as asm into human-readable form */
            wxString DumpCode();

            /** \brief Dump arguments into himan-readable form */
            wxString DumpArgs();

        private:

            std::vector< Value >     m_CodeArguments;       ///< \brief list of arguments
            std::vector< Operation > m_Code;                ///< \brief the code
    };
}

#endif
