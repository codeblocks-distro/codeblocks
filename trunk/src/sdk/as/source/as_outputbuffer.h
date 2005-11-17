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
// as_outputbuffer.h
//
// This class appends strings to one large buffer that can later
// be sent to the real output stream
//


#ifndef AS_OUTPUTBUFFER_H
#define AS_OUTPUTBUFFER_H

#include "as_config.h"
#include "as_string.h"

BEGIN_AS_NAMESPACE

class asCOutputBuffer : public virtual asIOutputStream
{
public:
	virtual ~asCOutputBuffer() {}

	void Write(const char *text);

	asCString output;
};

END_AS_NAMESPACE

#endif
