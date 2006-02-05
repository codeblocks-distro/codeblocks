/*
   AngelCode Scripting Library
   Copyright (c) 2003-2006 Andreas J�nsson

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
// angelscript.h
//
// The script engine interface
//


#ifndef ANGELSCRIPT_H
#define ANGELSCRIPT_H

#include <stddef.h>

#ifdef AS_USE_NAMESPACE
 #define BEGIN_AS_NAMESPACE namespace AngelScript {
 #define END_AS_NAMESPACE }
#else
 #define BEGIN_AS_NAMESPACE
 #define END_AS_NAMESPACE
#endif

BEGIN_AS_NAMESPACE

// AngelScript version

#define ANGELSCRIPT_VERSION        20500
#define ANGELSCRIPT_VERSION_MAJOR  2
#define ANGELSCRIPT_VERSION_MINOR  5
#define ANGELSCRIPT_VERSION_BUILD  0
#define ANGELSCRIPT_VERSION_STRING "2.5.0b"

// Data types

class asIScriptEngine;
class asIScriptContext;
class asIScriptGeneric;
class asIScriptAny;
class asIScriptStruct;
class asIScriptArray;
class asIOutputStream;
class asIBinaryStream;
typedef unsigned long  asDWORD;
#if defined(__GNUC__) || defined(__MWERKS__)
typedef long long asQWORD;
#else
typedef __int64 asQWORD;
#endif
typedef unsigned short asWORD;
typedef unsigned char  asBYTE;
typedef unsigned int   asUINT;

typedef void (*asFUNCTION_t)();

typedef void *(*asALLOCFUNC_t)(size_t);
typedef void (*asFREEFUNC_t)(void *);

#define asFUNCTION(f) asFunctionPtr((void (*)())(f))
#define asFUNCTIONPR(f,p,r) asFunctionPtr((void (*)())((r (*)p)(f)))

#define asMODULEIDX(id) ((id >> 16) & 0x3FF)

#ifndef AS_NO_CLASS_METHODS

class asCUnknownClass;
typedef void (asCUnknownClass::*asMETHOD_t)();

union asUPtr
{
	char dummy[24]; // largest known class method pointer
	struct {asMETHOD_t   mthd; char dummy[24-sizeof(asMETHOD_t)];} m;
	struct {asFUNCTION_t func; char dummy[24-sizeof(asFUNCTION_t)];} f;
};

#define asMETHOD(c,m) asSMethodPtr<sizeof(void (c::*)())>::Convert((void (c::*)())(&c::m))
#define asMETHODPR(c,m,p,r) asSMethodPtr<sizeof(void (c::*)())>::Convert((r (c::*)p)(&c::m))

#else // Class methods are disabled

union asUPtr
{
	char         dummy[20]; // largest known class method pointer
	asFUNCTION_t func;
};

#endif


typedef void (*asOUTPUTFUNC_t)(const char *text, void *param);
#ifdef AS_C_INTERFACE
typedef void (*asBINARYREADFUNC_t)(void *ptr, asUINT size, void *param);
typedef void (*asBINARYWRITEFUNC_t)(const void *ptr, asUINT size, void *param);
#endif

// API functions

// ANGELSCRIPT_EXPORT is defined when compiling the dll or lib
// ANGELSCRIPT_DLL_LIBRARY_IMPORT is defined when dynamically linking to the
// dll through the link lib automatically generated by MSVC++
// ANGELSCRIPT_DLL_MANUAL_IMPORT is defined when manually loading the dll
// Don't define anything when linking statically to the lib

#ifdef WIN32
  #ifdef ANGELSCRIPT_EXPORT
    #define AS_API __declspec(dllexport)
  #elif defined ANGELSCRIPT_DLL_LIBRARY_IMPORT
    #define AS_API __declspec(dllimport)
  #else // statically linked library
    #define AS_API
  #endif
#else
  #define AS_API
#endif

#ifndef ANGELSCRIPT_DLL_MANUAL_IMPORT
extern "C"
{
	// Engine
	AS_API asIScriptEngine * asCreateScriptEngine(asDWORD version);
	AS_API const char * asGetLibraryVersion();

	// Context
	AS_API asIScriptContext * asGetActiveContext();

	// Thread support
	AS_API int asThreadCleanup();

#ifdef AS_C_INTERFACE
	AS_API int               asEngine_AddRef(asIScriptEngine *e);
	AS_API int               asEngine_Release(asIScriptEngine *e);
	AS_API void              asEngine_SetCommonMessageStream(asIScriptEngine *e, asOUTPUTFUNC_t outFunc, void *outParam = 0);
	AS_API int               asEngine_SetCommonObjectMemoryFunctions(asIScriptEngine *e, asALLOCFUNC_t allocFunc, asFREEFUNC_t freeFunc);
	AS_API int               asEngine_RegisterObjectType(asIScriptEngine *e, const char *name, int byteSize, asDWORD flags);
	AS_API int               asEngine_RegisterObjectProperty(asIScriptEngine *e, const char *obj, const char *declaration, int byteOffset);
	AS_API int               asEngine_RegisterObjectMethod(asIScriptEngine *e, const char *obj, const char *declaration, asFUNCTION_t funcPointer, asDWORD callConv);
	AS_API int               asEngine_RegisterObjectBehaviour(asIScriptEngine *e, const char *datatype, asDWORD behaviour, const char *declaration, asFUNCTION_t funcPointer, asDWORD callConv);
	AS_API int               asEngine_RegisterGlobalProperty(asIScriptEngine *e, const char *declaration, void *pointer);
	AS_API int               asEngine_RegisterGlobalFunction(asIScriptEngine *e, const char *declaration, asFUNCTION_t funcPointer, asDWORD callConv);
	AS_API int               asEngine_RegisterGlobalBehaviour(asIScriptEngine *e, asDWORD behaviour, const char *declaration, asFUNCTION_t funcPointer, asDWORD callConv);
	AS_API int               asEngine_RegisterStringFactory(asIScriptEngine *e, const char *datatype, asFUNCTION_t factoryFunc, asDWORD callConv);
	AS_API int               asEngine_BeginConfigGroup(asIScriptEngine *e, const char *groupName);
	AS_API int               asEngine_EndConfigGroup(asIScriptEngine *e);
	AS_API int               asEngine_RemoveConfigGroup(asIScriptEngine *e, const char *groupName);
	AS_API int               asEngine_SetConfigGroupModuleAccess(asIScriptEngine *e, const char *groupName, const char *module, bool hasAccess);
	AS_API int               asEngine_AddScriptSection(asIScriptEngine *e, const char *module, const char *name, const char *code, int codeLength, int lineOffset = 0, bool makeCopy = true);
	AS_API int               asEngine_Build(asIScriptEngine *e, const char *module);
	AS_API int               asEngine_Discard(asIScriptEngine *e, const char *module);
	AS_API int               asEngine_GetModuleIndex(asIScriptEngine *e, const char *module);
	AS_API const char *      asEngine_GetModuleNameFromIndex(asIScriptEngine *e, int index, int *length = 0);
	AS_API int               asEngine_GetFunctionCount(asIScriptEngine *e, const char *module);
	AS_API int               asEngine_GetFunctionIDByIndex(asIScriptEngine *e, const char *module, int index);
	AS_API int               asEngine_GetFunctionIDByName(asIScriptEngine *e, const char *module, const char *name);
	AS_API int               asEngine_GetFunctionIDByDecl(asIScriptEngine *e, const char *module, const char *decl);
	AS_API const char *      asEngine_GetFunctionDeclaration(asIScriptEngine *e, int funcID, int *length = 0);
	AS_API const char *      asEngine_GetFunctionName(asIScriptEngine *e, int funcID, int *length = 0);
	AS_API const char *      asEngine_GetFunctionSection(asIScriptEngine *e, int funcID, int *length = 0);
	AS_API int               asEngine_GetGlobalVarCount(asIScriptEngine *e, const char *module);
	AS_API int               asEngine_GetGlobalVarIDByIndex(asIScriptEngine *e, const char *module, int index);
	AS_API int               asEngine_GetGlobalVarIDByName(asIScriptEngine *e, const char *module, const char *name);
	AS_API int               asEngine_GetGlobalVarIDByDecl(asIScriptEngine *e, const char *module, const char *decl);
	AS_API const char *      asEngine_GetGlobalVarDeclaration(asIScriptEngine *e, int gvarID, int *length = 0);
	AS_API const char *      asEngine_GetGlobalVarName(asIScriptEngine *e, int gvarID, int *length = 0);
	AS_API void *            asEngine_GetGlobalVarPointer(asIScriptEngine *e, int gvarID);
	AS_API int               asEngine_GetImportedFunctionCount(asIScriptEngine *e, const char *module);
	AS_API int               asEngine_GetImportedFunctionIndexByDecl(asIScriptEngine *e, const char *module, const char *decl);
	AS_API const char *      asEngine_GetImportedFunctionDeclaration(asIScriptEngine *e, const char *module, int importIndex, int *length = 0);
	AS_API const char *      asEngine_GetImportedFunctionSourceModule(asIScriptEngine *e, const char *module, int importIndex, int *length = 0);
	AS_API int               asEngine_BindImportedFunction(asIScriptEngine *e, const char *module, int importIndex, int funcID);
	AS_API int               asEngine_UnbindImportedFunction(asIScriptEngine *e, const char *module, int importIndex);
	AS_API int               asEngine_BindAllImportedFunctions(asIScriptEngine *e, const char *module);
	AS_API int               asEngine_UnbindAllImportedFunctions(asIScriptEngine *e, const char *module);
	AS_API int               asEngine_GetTypeIdByDecl(asIScriptEngine *e, const char *module, const char *decl);
	AS_API const char *      asEngine_GetTypeDeclaration(asIScriptEngine *e, int typeId, int *length = 0);
	AS_API int               asEngine_SetDefaultContextStackSize(asIScriptEngine *e, asUINT initial, asUINT maximum);
	AS_API asIScriptContext *asEngine_CreateContext(asIScriptEngine *e);
	AS_API void *            asEngine_CreateScriptObject(asIScriptEngine *e, int typeId);
	AS_API int               asEngine_ExecuteString(asIScriptEngine *e, const char *module, const char *script, asIScriptContext **ctx, asDWORD flags);
	AS_API int               asEngine_GarbageCollect(asIScriptEngine *e, bool doFullCycle = true);
	AS_API int               asEngine_GetObjectsInGarbageCollectorCount(asIScriptEngine *e);
	AS_API int               asEngine_SaveByteCode(asIScriptEngine *e, const char *module, asBINARYWRITEFUNC_t outFunc, void *outParam);
	AS_API int               asEngine_LoadByteCode(asIScriptEngine *e, const char *module, asBINARYREADFUNC_t inFunc, void *inParam);

	AS_API int              asContext_AddRef(asIScriptContext *c);
	AS_API int              asContext_Release(asIScriptContext *c);
	AS_API asIScriptEngine *asContext_GetEngine(asIScriptContext *c);
	AS_API int              asContext_GetState(asIScriptContext *c);
	AS_API int              asContext_Prepare(asIScriptContext *c, int funcID);
	AS_API int              asContext_SetArgDWord(asIScriptContext *c, asUINT arg, asDWORD value);
	AS_API int              asContext_SetArgQWord(asIScriptContext *c, asUINT arg, asQWORD value);
	AS_API int              asContext_SetArgFloat(asIScriptContext *c, asUINT arg, float value);
	AS_API int              asContext_SetArgDouble(asIScriptContext *c, asUINT arg, double value);
	AS_API int              asContext_SetArgObject(asIScriptContext *c, asUINT arg, void *obj);
	AS_API asDWORD          asContext_GetReturnDWord(asIScriptContext *c);
	AS_API asQWORD          asContext_GetReturnQWord(asIScriptContext *c);
	AS_API float            asContext_GetReturnFloat(asIScriptContext *c);
	AS_API double           asContext_GetReturnDouble(asIScriptContext *c);
	AS_API void *           asContext_GetReturnObject(asIScriptContext *c);
	AS_API int              asContext_Execute(asIScriptContext *c);
	AS_API int              asContext_Abort(asIScriptContext *c);
	AS_API int              asContext_Suspend(asIScriptContext *c);
	AS_API int              asContext_GetCurrentLineNumber(asIScriptContext *c, int *column = 0);
	AS_API int              asContext_GetCurrentFunction(asIScriptContext *c);
	AS_API int              asContext_SetException(asIScriptContext *c, const char *string);
	AS_API int              asContext_GetExceptionLineNumber(asIScriptContext *c, int *column = 0);
	AS_API int              asContext_GetExceptionFunction(asIScriptContext *c);
	AS_API const char *     asContext_GetExceptionString(asIScriptContext *c, int *length = 0);
	AS_API int              asContext_SetLineCallback(asIScriptContext *c, asUPtr callback, void *obj, int callConv);
	AS_API void             asContext_ClearLineCallback(asIScriptContext *c);
	AS_API int              asContext_SetExceptionCallback(asIScriptContext *c, asUPtr callback, void *obj, int callConv);
	AS_API void             asContext_ClearExceptionCallback(asIScriptContext *c);
	AS_API int              asContext_GetCallstackSize(asIScriptContext *c);
	AS_API int              asContext_GetCallstackFunction(asIScriptContext *c, int index);
	AS_API int              asContext_GetCallstackLineNumber(asIScriptContext *c, int index, int *column = 0);
	AS_API int              asContext_GetVarCount(asIScriptContext *c, int stackLevel = 0);
	AS_API const char *     asContext_GetVarName(asIScriptContext *c, int varIndex, int *length = 0, int stackLevel = 0);
	AS_API const char *     asContext_GetVarDeclaration(asIScriptContext *c, int varIndex, int *length = 0, int stackLevel = 0);
	AS_API void *           asContext_GetVarPointer(asIScriptContext *c, int varIndex, int stackLevel = 0);


	AS_API asIScriptEngine *asGeneric_GetEngine(asIScriptGeneric *g);
	AS_API void *           asGeneric_GetObject(asIScriptGeneric *g);
	AS_API asDWORD          asGeneric_GetArgDWord(asIScriptGeneric *g, asUINT arg);
	AS_API asQWORD          asGeneric_GetArgQWord(asIScriptGeneric *g, asUINT arg);
	AS_API float            asGeneric_GetArgFloat(asIScriptGeneric *g, asUINT arg);
	AS_API double           asGeneric_GetArgDouble(asIScriptGeneric *g, asUINT arg);
	AS_API void *           asGeneric_GetArgObject(asIScriptGeneric *g, asUINT arg);
	AS_API int              asGeneric_SetReturnDWord(asIScriptGeneric *g, asDWORD val);
	AS_API int              asGeneric_SetReturnQWord(asIScriptGeneric *g, asQWORD val);
	AS_API int              asGeneric_SetReturnFloat(asIScriptGeneric *g, float val);
	AS_API int              asGeneric_SetReturnDouble(asIScriptGeneric *g, double val);
	AS_API int              asGeneric_SetReturnObject(asIScriptGeneric *g, void *obj);

	AS_API int  asAny_AddRef(asIScriptAny *a);
	AS_API int  asAny_Release(asIScriptAny *a);
	AS_API void asAny_Store(asIScriptAny *a, void *ref, int typeId);
	AS_API int  asAny_Retrieve(asIScriptAny *a, void *ref, int typeId);
	AS_API int  asAny_GetTypeId(asIScriptAny *a);
	AS_API int  asAny_CopyFrom(asIScriptAny *a, asIScriptAny *other);

	AS_API int         asStruct_AddRef(asIScriptStruct *s);
	AS_API int         asStruct_Release(asIScriptStruct *s);
	AS_API int         asStruct_GetStructTypeId(asIScriptStruct *s);
	AS_API int         asStruct_GetPropertyCount(asIScriptStruct *s);
	AS_API int         asStruct_GetPropertyTypeId(asIScriptStruct *s, asUINT prop);
	AS_API const char *asStruct_GetPropertyName(asIScriptStruct *s, asUINT prop);
	AS_API void *      asStruct_GetPropertyPointer(asIScriptStruct *s, asUINT prop);
	AS_API int         asStruct_CopyFrom(asIScriptStruct *s, asIScriptStruct *other);

	AS_API int    asArray_AddRef(asIScriptArray *a);
	AS_API int    asArray_Release(asIScriptArray *a);
	AS_API int    asArray_GetArrayTypeId(asIScriptArray *a);
	AS_API int    asArray_GetElementTypeId(asIScriptArray *a);
	AS_API asUINT asArray_GetElementCount(asIScriptArray *a);
	AS_API void * asArray_GetElementPointer(asIScriptArray *a, asUINT index);
	AS_API void   asArray_Resize(asIScriptArray *a, asUINT size);
	AS_API int    asArray_CopyFrom(asIScriptArray *a, asIScriptArray *other);

#endif // AS_C_INTERFACE
}
#endif // ANGELSCRIPT_DLL_MANUAL_IMPORT

// Interface declarations

class asIScriptEngine
{
public:
	// Memory management
	virtual int AddRef() = 0;
	virtual int Release() = 0;

	// Engine configuration
	virtual void SetCommonMessageStream(asIOutputStream *out) = 0;
	virtual void SetCommonMessageStream(asOUTPUTFUNC_t outFunc, void *outParam) = 0;
	virtual int SetCommonObjectMemoryFunctions(asALLOCFUNC_t allocFunc, asFREEFUNC_t freeFunc) = 0;

	virtual int RegisterObjectType(const char *name, int byteSize, asDWORD flags) = 0;
	virtual int RegisterObjectProperty(const char *obj, const char *declaration, int byteOffset) = 0;
	virtual int RegisterObjectMethod(const char *obj, const char *declaration, const asUPtr &funcPointer, asDWORD callConv) = 0;
	virtual int RegisterObjectBehaviour(const char *datatype, asDWORD behaviour, const char *declaration, const asUPtr &funcPointer, asDWORD callConv) = 0;

	virtual int RegisterGlobalProperty(const char *declaration, void *pointer) = 0;
	virtual int RegisterGlobalFunction(const char *declaration, const asUPtr &funcPointer, asDWORD callConv) = 0;
	virtual int RegisterGlobalBehaviour(asDWORD behaviour, const char *declaration, const asUPtr &funcPointer, asDWORD callConv) = 0;

	virtual int RegisterStringFactory(const char *datatype, const asUPtr &factoryFunc, asDWORD callConv) = 0;

	virtual int BeginConfigGroup(const char *groupName) = 0;
	virtual int EndConfigGroup() = 0;
	virtual int RemoveConfigGroup(const char *groupName) = 0;
	virtual int SetConfigGroupModuleAccess(const char *groupName, const char *module, bool hasAccess) = 0;

	// Script modules
	virtual int AddScriptSection(const char *module, const char *name, const char *code, int codeLength, int lineOffset = 0, bool makeCopy = true) = 0;
#ifdef AS_DEPRECATED
	virtual int Build(const char *module, asIOutputStream *out) = 0;
#endif
	virtual int Build(const char *module) = 0;
    virtual int Discard(const char *module) = 0;
	virtual int ResetModule(const char *module) = 0;
	virtual int GetModuleIndex(const char *module) = 0;
	virtual const char *GetModuleNameFromIndex(int index, int *length = 0) = 0;

	// Script functions
	virtual int GetFunctionCount(const char *module) = 0;
	virtual int GetFunctionIDByIndex(const char *module, int index) = 0;
	virtual int GetFunctionIDByName(const char *module, const char *name) = 0;
	virtual int GetFunctionIDByDecl(const char *module, const char *decl) = 0;
	virtual const char *GetFunctionDeclaration(int funcID, int *length = 0) = 0;
	virtual const char *GetFunctionName(int funcID, int *length = 0) = 0;
	virtual const char *GetFunctionSection(int funcID, int *length = 0) = 0;

	// Script global variables
	virtual int GetGlobalVarCount(const char *module) = 0;
	virtual int GetGlobalVarIDByIndex(const char *module, int index) = 0;
	virtual int GetGlobalVarIDByName(const char *module, const char *name) = 0;
	virtual int GetGlobalVarIDByDecl(const char *module, const char *decl) = 0;
	virtual const char *GetGlobalVarDeclaration(int gvarID, int *length = 0) = 0;
	virtual const char *GetGlobalVarName(int gvarID, int *length = 0) = 0;
	virtual void *GetGlobalVarPointer(int gvarID) = 0;
#ifdef AS_DEPRECATED
	virtual int GetGlobalVarPointer(int gvarID, void **ptr) = 0;
#endif

	// Dynamic binding between modules
	virtual int GetImportedFunctionCount(const char *module) = 0;
	virtual int GetImportedFunctionIndexByDecl(const char *module, const char *decl) = 0;
	virtual const char *GetImportedFunctionDeclaration(const char *module, int importIndex, int *length = 0) = 0;
	virtual const char *GetImportedFunctionSourceModule(const char *module, int importIndex, int *length = 0) = 0;
	virtual int BindImportedFunction(const char *module, int importIndex, int funcID) = 0;
	virtual int UnbindImportedFunction(const char *module, int importIndex) = 0;

	virtual int BindAllImportedFunctions(const char *module) = 0;
	virtual int UnbindAllImportedFunctions(const char *module) = 0;

	// Type identification
	virtual int GetTypeIdByDecl(const char *module, const char *decl) = 0;
	virtual const char *GetTypeDeclaration(int typeId, int *length = 0) = 0;

	// Script execution
	virtual int SetDefaultContextStackSize(asUINT initial, asUINT maximum) = 0;
	virtual asIScriptContext *CreateContext() = 0;
#ifdef AS_DEPRECATED
	virtual int CreateContext(asIScriptContext **ctx) = 0;
#endif
	virtual void *CreateScriptObject(int typeId) = 0;

	// String interpretation
#ifdef AS_DEPRECATED
	virtual int ExecuteString(const char *module, const char *script, asIOutputStream *out, asIScriptContext **ctx = 0, asDWORD flags = 0) = 0;
#endif
	virtual int ExecuteString(const char *module, const char *script, asIScriptContext **ctx = 0, asDWORD flags = 0) = 0;

	// Garbage collection
	virtual int GarbageCollect(bool doFullCycle = true) = 0;
	virtual int GetObjectsInGarbageCollectorCount() = 0;

	// Bytecode Saving/Loading
	virtual int SaveByteCode(const char *module, asIBinaryStream *out) = 0;
	virtual int LoadByteCode(const char *module, asIBinaryStream *in) = 0;

protected:
	virtual ~asIScriptEngine() {};
};

class asIScriptContext
{
public:
	// Memory management
	virtual int AddRef() = 0;
	virtual int Release() = 0;

	// Engine
	virtual asIScriptEngine *GetEngine() = 0;

	// Script context
	virtual int GetState() = 0;

	virtual int Prepare(int funcID) = 0;

	virtual int SetArgDWord(asUINT arg, asDWORD value) = 0;
	virtual int SetArgQWord(asUINT arg, asQWORD value) = 0;
	virtual int SetArgFloat(asUINT arg, float value) = 0;
	virtual int SetArgDouble(asUINT arg, double value) = 0;
	virtual int SetArgObject(asUINT arg, void *obj) = 0;

	virtual asDWORD GetReturnDWord() = 0;
	virtual asQWORD GetReturnQWord() = 0;
	virtual float   GetReturnFloat() = 0;
	virtual double  GetReturnDouble() = 0;
	virtual void   *GetReturnObject() = 0;

	virtual int Execute() = 0;
	virtual int Abort() = 0;
	virtual int Suspend() = 0;

	virtual int GetCurrentLineNumber(int *column = 0) = 0;
	virtual int GetCurrentFunction() = 0;

	// Exception handling
	virtual int SetException(const char *string) = 0;
	virtual int GetExceptionLineNumber(int *column = 0) = 0;
	virtual int GetExceptionFunction() = 0;
	virtual const char *GetExceptionString(int *length = 0) = 0;

	virtual int  SetLineCallback(asUPtr callback, void *obj, int callConv) = 0;
	virtual void ClearLineCallback() = 0;
	virtual int  SetExceptionCallback(asUPtr callback, void *obj, int callConv) = 0;
	virtual void ClearExceptionCallback() = 0;

	virtual int GetCallstackSize() = 0;
	virtual int GetCallstackFunction(int index) = 0;
	virtual int GetCallstackLineNumber(int index, int *column = 0) = 0;

	virtual int GetVarCount(int stackLevel = -1) = 0;
	virtual const char *GetVarName(int varIndex, int *length = 0, int stackLevel = -1) = 0;
	virtual const char *GetVarDeclaration(int varIndex, int *length = 0, int stackLevel = -1) = 0;
	virtual void *GetVarPointer(int varIndex, int stackLevel = -1) = 0;

protected:
	virtual ~asIScriptContext() {};
};

class asIScriptGeneric
{
public:
	virtual asIScriptEngine *GetEngine() = 0;

	virtual void   *GetObject() = 0;

	virtual asDWORD GetArgDWord(asUINT arg) = 0;
	virtual asQWORD GetArgQWord(asUINT arg) = 0;
	virtual float   GetArgFloat(asUINT arg) = 0;
	virtual double  GetArgDouble(asUINT arg) = 0;
	virtual void   *GetArgObject(asUINT arg) = 0;

	virtual int     SetReturnDWord(asDWORD val) = 0;
	virtual int     SetReturnQWord(asQWORD val) = 0;
	virtual int     SetReturnFloat(float val) = 0;
	virtual int     SetReturnDouble(double val) = 0;
	virtual int     SetReturnObject(void *obj) = 0;

protected:
	virtual ~asIScriptGeneric() {};
};

class asIScriptAny
{
public:
	// Memory management
	virtual int AddRef() = 0;
	virtual int Release() = 0;

	// Contained value
	virtual void Store(void *ref, int typeId) = 0;
	virtual int  Retrieve(void *ref, int typeId) = 0;
	virtual int  GetTypeId() = 0;
	virtual int  CopyFrom(asIScriptAny *other) = 0;

protected:
	virtual ~asIScriptAny() {}
};

class asIScriptStruct
{
public:
	// Memory management
	virtual int AddRef() = 0;
	virtual int Release() = 0;

	// Struct type
	virtual int GetStructTypeId() = 0;

	// Struct properties
	virtual int GetPropertyCount() = 0;
	virtual int GetPropertyTypeId(asUINT prop) = 0;
	virtual const char *GetPropertyName(asUINT prop) = 0;
	virtual void *GetPropertyPointer(asUINT prop) = 0;
	virtual int CopyFrom(asIScriptStruct *other) = 0;

protected:
	virtual ~asIScriptStruct() {}
};

class asIScriptArray
{
public:
	// Memory management
	virtual int AddRef() = 0;
	virtual int Release() = 0;

	// Array type
	virtual int GetArrayTypeId() = 0;

	// Elements
	virtual int    GetElementTypeId() = 0;
	virtual asUINT GetElementCount() = 0;
	virtual void * GetElementPointer(asUINT index) = 0;
	virtual void   Resize(asUINT size) = 0;
	virtual int    CopyFrom(asIScriptArray *other) = 0;

protected:
	virtual ~asIScriptArray() {}
};

class asIOutputStream
{
public:
    virtual ~asIOutputStream() {}
	virtual void Write(const char *text) = 0;
};

class asIBinaryStream
{
public:
    virtual ~asIBinaryStream() {}
	virtual void Read(void *ptr, asUINT size) = 0;
	virtual void Write(const void *ptr, asUINT size) = 0;
};

// Enumerations and constants

// Calling conventions and flags

const asDWORD asCALL_CDECL            = 0;
const asDWORD asCALL_STDCALL          = 1;
const asDWORD asCALL_THISCALL         = 2;
const asDWORD asCALL_CDECL_OBJLAST    = 3;
const asDWORD asCALL_CDECL_OBJFIRST   = 4;
const asDWORD asCALL_GENERIC          = 5;

// Object type flags

const asDWORD asOBJ_CLASS             = 1;
const asDWORD asOBJ_CLASS_CONSTRUCTOR = 2;
const asDWORD asOBJ_CLASS_DESTRUCTOR  = 4;
const asDWORD asOBJ_CLASS_ASSIGNMENT  = 8;
const asDWORD asOBJ_CLASS_C           = (asOBJ_CLASS + asOBJ_CLASS_CONSTRUCTOR);
const asDWORD asOBJ_CLASS_CD          = (asOBJ_CLASS + asOBJ_CLASS_CONSTRUCTOR + asOBJ_CLASS_DESTRUCTOR);
const asDWORD asOBJ_CLASS_CA          = (asOBJ_CLASS + asOBJ_CLASS_CONSTRUCTOR + asOBJ_CLASS_ASSIGNMENT);
const asDWORD asOBJ_CLASS_CDA         = (asOBJ_CLASS + asOBJ_CLASS_CONSTRUCTOR + asOBJ_CLASS_DESTRUCTOR + asOBJ_CLASS_ASSIGNMENT);
const asDWORD asOBJ_CLASS_D           = (asOBJ_CLASS + asOBJ_CLASS_DESTRUCTOR);
const asDWORD asOBJ_CLASS_A           = (asOBJ_CLASS + asOBJ_CLASS_ASSIGNMENT);
const asDWORD asOBJ_CLASS_DA          = (asOBJ_CLASS + asOBJ_CLASS_DESTRUCTOR + asOBJ_CLASS_ASSIGNMENT);
const asDWORD asOBJ_PRIMITIVE         = 16;
const asDWORD asOBJ_FLOAT             = 17;

// Behaviours

const asDWORD asBEHAVE_CONSTRUCT     = 0;
const asDWORD asBEHAVE_DESTRUCT      = 1;
const asDWORD asBEHAVE_FIRST_ASSIGN  = 2;
 const asDWORD asBEHAVE_ASSIGNMENT    = 2;
 const asDWORD asBEHAVE_ADD_ASSIGN    = 3;
 const asDWORD asBEHAVE_SUB_ASSIGN    = 4;
 const asDWORD asBEHAVE_MUL_ASSIGN    = 5;
 const asDWORD asBEHAVE_DIV_ASSIGN    = 6;
 const asDWORD asBEHAVE_MOD_ASSIGN    = 7;
 const asDWORD asBEHAVE_OR_ASSIGN     = 8;
 const asDWORD asBEHAVE_AND_ASSIGN    = 9;
 const asDWORD asBEHAVE_XOR_ASSIGN    = 10;
 const asDWORD asBEHAVE_SLL_ASSIGN    = 11;
 const asDWORD asBEHAVE_SRL_ASSIGN    = 12;
 const asDWORD asBEHAVE_SRA_ASSIGN    = 13;
const asDWORD asBEHAVE_LAST_ASSIGN   = 13;
const asDWORD asBEHAVE_FIRST_DUAL    = 14;
 const asDWORD asBEHAVE_ADD           = 14;
 const asDWORD asBEHAVE_SUBTRACT      = 15;
 const asDWORD asBEHAVE_MULTIPLY      = 16;
 const asDWORD asBEHAVE_DIVIDE        = 17;
 const asDWORD asBEHAVE_MODULO        = 18;
 const asDWORD asBEHAVE_EQUAL         = 19;
 const asDWORD asBEHAVE_NOTEQUAL      = 20;
 const asDWORD asBEHAVE_LESSTHAN      = 21;
 const asDWORD asBEHAVE_GREATERTHAN   = 22;
 const asDWORD asBEHAVE_LEQUAL        = 23;
 const asDWORD asBEHAVE_GEQUAL        = 24;
 const asDWORD asBEHAVE_LOGIC_OR      = 25;
 const asDWORD asBEHAVE_LOGIC_AND     = 26;
 const asDWORD asBEHAVE_BIT_OR        = 27;
 const asDWORD asBEHAVE_BIT_AND       = 28;
 const asDWORD asBEHAVE_BIT_XOR       = 29;
 const asDWORD asBEHAVE_BIT_SLL       = 30;
 const asDWORD asBEHAVE_BIT_SRL       = 31;
 const asDWORD asBEHAVE_BIT_SRA       = 32;
const asDWORD asBEHAVE_LAST_DUAL     = 32;
const asDWORD asBEHAVE_INDEX         = 33;
const asDWORD asBEHAVE_NEGATE        = 34;
const asDWORD asBEHAVE_ADDREF        = 35;
const asDWORD asBEHAVE_RELEASE       = 36;
const asDWORD asBEHAVE_ALLOC         = 37;
const asDWORD asBEHAVE_FREE          = 38;

// Return codes

const int asSUCCESS                              =  0;
const int asERROR                                = -1;
const int asCONTEXT_ACTIVE                       = -2;
const int asCONTEXT_NOT_FINISHED                 = -3;
const int asCONTEXT_NOT_PREPARED                 = -4;
const int asINVALID_ARG                          = -5;
const int asNO_FUNCTION                          = -6;
const int asNOT_SUPPORTED                        = -7;
const int asINVALID_NAME                         = -8;
const int asNAME_TAKEN                           = -9;
const int asINVALID_DECLARATION                  = -10;
const int asINVALID_OBJECT                       = -11;
const int asINVALID_TYPE                         = -12;
const int asALREADY_REGISTERED                   = -13;
const int asMULTIPLE_FUNCTIONS                   = -14;
const int asNO_MODULE                            = -15;
const int asNO_GLOBAL_VAR                        = -16;
const int asINVALID_CONFIGURATION                = -17;
const int asINVALID_INTERFACE                    = -18;
const int asCANT_BIND_ALL_FUNCTIONS              = -19;
const int asLOWER_ARRAY_DIMENSION_NOT_REGISTERED = -20;
const int asWRONG_CONFIG_GROUP                   = -21;
const int asCONFIG_GROUP_IS_IN_USE               = -22;

// Context states

const int asEXECUTION_FINISHED      = 0;
const int asEXECUTION_SUSPENDED     = 1;
const int asEXECUTION_ABORTED       = 2;
const int asEXECUTION_EXCEPTION     = 3;
const int asEXECUTION_PREPARED      = 4;
const int asEXECUTION_UNINITIALIZED = 5;
const int asEXECUTION_ACTIVE        = 6;
const int asEXECUTION_ERROR         = 7;

// ExecuteString flags

const asDWORD asEXECSTRING_ONLY_PREPARE	  = 1;
const asDWORD asEXECSTRING_USE_MY_CONTEXT = 2;

// Prepare flags

const int asPREPARE_PREVIOUS = -1;

// Config groups

const char * const asALL_MODULES = (const char * const)-1;

// Type id flags

const int asTYPEID_OBJHANDLE      = 0x40000000;
const int asTYPEID_HANDLETOCONST  = 0x20000000;
const int asTYPEID_MASK_OBJECT    = 0x1C000000;
const int  asTYPEID_APPOBJECT      = 0x04000000;
const int  asTYPEID_SCRIPTANY      = 0x08000000;
const int  asTYPEID_SCRIPTSTRUCT   = 0x0C000000;
const int  asTYPEID_SCRIPTARRAY    = 0x10000000;
const int asTYPEID_MASK_SEQNBR    = 0x03FFFFFF;

//-----------------------------------------------------------------
// Function pointers

// Use our own memset() and memcpy() implementations for better portability
inline void asMemClear(void *_p, int size)
{
	char *p = (char *)_p;
	const char *e = p + size;
	for( ; p < e; p++ )
		*p = 0;
}

inline void asMemCopy(void *_d, const void *_s, int size)
{
	char *d = (char *)_d;
	const char *s = (const char *)_s;
	const char *e = s + size;
	for( ; s < e; d++, s++ )
		*d = *s;
}

inline asUPtr asFunctionPtr(asFUNCTION_t func)
{
	asUPtr p;
	asMemClear(&p, sizeof(p));
	p.f.func = func;

	return p;
}

#ifndef AS_NO_CLASS_METHODS

// Method pointers

// Declare a dummy class so that we can determine the size of a simple method pointer
class asCSimpleDummy {};
typedef void (asCSimpleDummy::*asSIMPLEMETHOD_t)();
const int SINGLE_PTR_SIZE = sizeof(asSIMPLEMETHOD_t);

// Define template
template <int N>
struct asSMethodPtr
{
	template<class M>
	static asUPtr Convert(M Mthd)
	{
		// This version of the function should never be executed, nor compiled,
		// as it would mean that the size of the method pointer cannot be determined.
		// int ERROR_UnsupportedMethodPtr[-1];
		return 0;
	}
};

// Template specialization
template <>
struct asSMethodPtr<SINGLE_PTR_SIZE>
{
	template<class M>
	static asUPtr Convert(M Mthd)
	{
		asUPtr p;
		asMemClear(&p, sizeof(p));

		asMemCopy(&p, &Mthd, SINGLE_PTR_SIZE);

		return p;
	}
};

#if defined(_MSC_VER) && !defined(__MWERKS__)

// MSVC and Intel uses different sizes for different class method pointers
template <>
struct asSMethodPtr<SINGLE_PTR_SIZE+1*sizeof(int)>
{
	template <class M>
	static asUPtr Convert(M Mthd)
	{
		asUPtr p;
		asMemClear(&p, sizeof(p));

		asMemCopy(&p, &Mthd, SINGLE_PTR_SIZE+sizeof(int));

		return p;
	}
};

template <>
struct asSMethodPtr<SINGLE_PTR_SIZE+2*sizeof(int)>
{
	template <class M>
	static asUPtr Convert(M Mthd)
	{
		// This is where a class with virtual inheritance falls

		// Since method pointers of this type doesn't have all the
		// information we need we force a compile failure for this case.
		int ERROR_VirtualInheritanceIsNotAllowedForMSVC[-1];

		// The missing information is the location of the vbase table,
		// which is only known at compile time.

		// You can get around this by forward declaring the class and
		// storing the sizeof its method pointer in a constant. Example:

		// class ClassWithVirtualInheritance;
		// const int ClassWithVirtualInheritance_workaround = sizeof(void ClassWithVirtualInheritance::*());

		// This will force the compiler to use the unknown type
		// for the class, which falls under the next case

		asUPtr p;
		return p;
	}
};

template <>
struct asSMethodPtr<SINGLE_PTR_SIZE+3*sizeof(int)>
{
	template <class M>
	static asUPtr Convert(M Mthd)
	{
		asUPtr p;
		asMemClear(&p, sizeof(p));

		asMemCopy(&p, &Mthd, SINGLE_PTR_SIZE+3*sizeof(int));

		return p;
	}
};

#endif

#endif // AS_NO_CLASS_METHODS

END_AS_NAMESPACE

#endif
